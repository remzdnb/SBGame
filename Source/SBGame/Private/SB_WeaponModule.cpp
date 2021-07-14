#include "SB_WeaponModule.h"
#include "SB_WeaponModuleAnimInstance.h"
#include "SB_Ship.h"
#include "SB_Projectile.h"
#include "SB_DataManager.h"
//
#include "Engine/SkeletalMesh.h"
#include "Components/SceneComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

#pragma region +++++ Setup ...

USB_WeaponModule::USB_WeaponModule()
{
	bIsSelected = false;
	bIsSniperView = false;
	bWantsToFire = false;
	bIsFiring = false;
	LastFireTime = 0.0f;
}

void USB_WeaponModule::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetWorld()->IsGameWorld() == false)
		return;
	
	WeaponModuleData = DataManager->GetWeaponModuleDataFromRow(ModuleName);
	WeaponAnimInstance = Cast<USB_WeaponModuleAnimInstance>(GetAnimInstance());
}

#pragma endregion

#pragma region +++++ Main ...

void USB_WeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerShip == nullptr || WeaponModuleData == nullptr)
		return;

	SelectAutoLockCT();
	FireTick();

	if (WeaponAnimInstance)
	{
		WeaponAnimInstance->UpdateInstance(GetComponentLocation(), OwnerShip->GetOwnerViewLocation());
	}

	Debug(DeltaTime);
}

void USB_WeaponModule::SetIsSelected(bool bToggleSelection, bool bNewIsSelected)
{
	if (bToggleSelection)
	{
		bIsSelected = bIsSelected ? false : true;
		SetRenderCustomDepth(bIsSelected);
		ModuleSelectionUpdatedEvent.Broadcast(bIsSelected);
	}
	else
	{
		if (bIsSelected && !bNewIsSelected)
		{
			bIsSelected = false;
			SetRenderCustomDepth(false);
			ModuleSelectionUpdatedEvent.Broadcast(false);
		}
		else if (!bIsSelected && bNewIsSelected)
		{
			bIsSelected = true;
			SetRenderCustomDepth(true);
			ModuleSelectionUpdatedEvent.Broadcast(true);
		}
	}
}

void USB_WeaponModule::ToggleSniperView(bool bNewIsSniperView)
{
	bIsSniperView = bNewIsSniperView;
}

#pragma endregion

#pragma region +++++ AutoLock ...

void USB_WeaponModule::SetTargetShip(ASB_Ship* const NewTargetShip)
{
	if (TargetShip)
		TargetShip->ToggleOutline(false);

	if (NewTargetShip)
		NewTargetShip->ToggleOutline(true);

	TargetShip = NewTargetShip;
	TargetAutoLockCT = nullptr;
	bWantsToFire = false;
}

void USB_WeaponModule::SelectAutoLockCT()
{
	if (OwnerShip == nullptr)
		return;

	if (TargetShip == nullptr)
		return;

	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		return;

	TargetAutoLockCT = nullptr;

	TArray<FHitResult> Hits;
	const FVector Start = GetComponentLocation();
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	for (auto& AutoLockCT : TargetShip->GetAutoLockCTs())
	{
		const FVector End = AutoLockCT->GetComponentLocation();
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Green, GetWorld()->GetDeltaSeconds() + 0.01f, 20.0f);
		GetWorld()->LineTraceMultiByChannel(Hits, Start, End, TRACE_OVERLAP, TraceParams);
		for (FHitResult& Hit : Hits)
		{
			if (Hit.Actor == TargetShip)
			{
				TargetAutoLockCT = AutoLockCT;
				bWantsToFire = true;
				return;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Orange, *Hit.Actor->GetName());
				break;
			}
		}
	}

	bWantsToFire = false;
}

#pragma endregion

#pragma region +++++ Fire ...

void USB_WeaponModule::SetWantsToFire(bool bNewWantsToFire)
{
	if (TargetShip)
	{
		TargetShip->ToggleOutline(false);
		TargetShip = nullptr;
		TargetAutoLockCT = nullptr;
	}

	bWantsToFire = bNewWantsToFire;
}

void USB_WeaponModule::FireTick()
{
	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		return;

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if ((CurrentTime - LastFireTime) >= WeaponModuleData->FireRate)
	{
		if (bIsFiring)
		{
			bIsFiring = false;
		}
	}

	if (bWantsToFire && !bIsFiring)
	{
		FireOnce();
		bIsFiring = true;
		LastFireTime = CurrentTime;
	}
}

void USB_WeaponModule::FireOnce()
{
	for (uint8 MuzzleIndex = 1; MuzzleIndex <= WeaponModuleData->MuzzleCount; MuzzleIndex++)
	{
		const FName MuzzleSocketName = *("MuzzleSocket_0" + FString::FromInt(MuzzleIndex));
		const FVector StartLocation = GetSocketLocation(MuzzleSocketName);
		const FVector TargetLocation = TargetAutoLockCT ? TargetAutoLockCT->GetComponentLocation() : OwnerShip->GetOwnerViewLocation();
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		const FTransform SpawnTransform(SpawnRotation, StartLocation, FVector(1.0f));
		const FActorSpawnParameters SpawnParameters;

		ASB_Projectile* const Projectile = GetWorld()->SpawnActorDeferred<ASB_Projectile>(WeaponModuleData->ProjectileBP, SpawnTransform, GetOwner(), nullptr);
		if (Projectile)
		{
			//Projectile->Init(WeaponModuleData, (ASB_Ship*)GetOwner());
			Projectile->GetProjectileMovementCT()->InitialSpeed = 100000.0f;
			Projectile->GetProjectileMovementCT()->MaxSpeed = 100000.0f;
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

			if (DataManager->GameSettings.bIsDebugEnabled_WeaponModule)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "USB_WeaponModule::FireOnce : Projectile spawned");
		}

		if (WeaponModuleData->MuzzleParticle)
		{
			MuzzleParticle = UGameplayStatics::SpawnEmitterAttached(
				WeaponModuleData->MuzzleParticle,
				this,
				MuzzleSocketName,
				FVector(0.0f),
				FRotator(0.0f),
				EAttachLocation::SnapToTarget
			);

			if (MuzzleParticle) // crash on dedicated obv
				MuzzleParticle->SetWorldScale3D(FVector(WeaponModuleData->MuzzleParticleScale));
		}
	}
}

#pragma endregion

#pragma region +++++ Network / Debug ...

void USB_WeaponModule::Debug(float DeltaTime)
{
	if (OwnerShip == nullptr)
		return;

	if (DataManager == nullptr)
		return;

	if (DataManager->GameSettings.bIsDebugEnabled_WeaponModule == false)
		return;

	FString RoleString = "None";
	FColor Color = FColor::White;

	if (OwnerShip->GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority :: ";
		Color = FColor::Cyan;
	}
	if (OwnerShip->GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy :: ";
		Color = FColor::Yellow;
	}
	if (OwnerShip->GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy :: ";
		Color = FColor::Orange;
	}

	FString TargetShipString;
	if (TargetShip)
		TargetShipString = TargetShip->GetName();
	else
		TargetShipString = "nullptr";

	FString TargetAutoLockString;
	if (TargetAutoLockCT)
		TargetAutoLockString = TargetAutoLockCT->GetName();
	else
		TargetAutoLockString = "nullptr";

	const FString StringToPrint = 
		RoleString + this->GetName() + 
		" // bWantsToFire = " + FString::FromInt(bWantsToFire) +
		" // bIsFiring = " + FString::FromInt(bIsFiring) +
		" // TargetShip = " + TargetShipString +
		" // TargetAutoLockCT = " + TargetAutoLockString;

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, StringToPrint);
}

#pragma endregion