#include "SB_WeaponModule.h"
#include "SB_WeaponModuleAnimInstance.h"
#include "SB_Ship.h"
#include "SB_ModuleSlot.h"
#include "SB_Projectile.h"
#include "SB_DataManager.h"
//
#include "Engine/SkeletalMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

USB_WeaponModule::USB_WeaponModule()
{
	SetCollisionProfileName("CharacterMesh");

	bWantsToFire = false;
	bIsFiring = false;
	LastFireTime = 0.0f;
}

void USB_WeaponModule::Init(const class ASB_DataManager* const NewDataManager, const FName& NewDataRowName)
{
	Super::Init(NewDataManager, NewDataRowName);

	WeaponModuleData = *NewDataManager->GetWeaponModuleDataFromRow(NewDataRowName);
	//AInstance = Cast<USB_WeaponModuleAnimInstance>(GetAnimInstance());
}

void USB_WeaponModule::ToggleSelection()
{
	if (bIsSelected)
		bIsSelected = false;
	else
		bIsSelected = true;

	ModuleSelectionUpdatedEvent.Broadcast(bIsSelected);
}

void USB_WeaponModule::SetWantsToFire(bool bNewWantsToFire)
{
	if (bIsSelected)
	{
		bWantsToFire = bNewWantsToFire;
	}
}

void USB_WeaponModule::BeginPlay()
{
	Super::BeginPlay();
}

void USB_WeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FireTick();

	USB_WeaponModuleAnimInstance* AInstance = Cast<USB_WeaponModuleAnimInstance>(GetAnimInstance());
	if (AInstance && OwnerShip)
	{
		AInstance->UpdateInstance(GetComponentLocation(), OwnerShip->GetShipTargetLocation());
	}

	Debug(DeltaTime);
}

void USB_WeaponModule::FireTick()
{
	if (OwnerShip == nullptr)
		return;

	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		return;

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if ((CurrentTime - LastFireTime) >= WeaponModuleData.FireRate)
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
	const FVector StartLocation = GetSocketLocation("MuzzleSocket");
	const FVector TargetLocation = OwnerShip->GetShipTargetLocation();
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	const FTransform SpawnTransform(SpawnRotation, StartLocation, FVector(1.0f));
	const FActorSpawnParameters SpawnParameters;

	ASB_Projectile* const Projectile = GetWorld()->SpawnActorDeferred<ASB_Projectile>(WeaponModuleData.ProjectileBP, SpawnTransform, GetOwner(), nullptr);
	if (Projectile)
	{
		//Projectile->Init(WeaponModuleData, (ASB_Ship*)GetOwner());
		Projectile->GetProjectileMovementCT()->InitialSpeed = 100000.0f;
		Projectile->GetProjectileMovementCT()->MaxSpeed = 100000.0f;
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

		if (DataManager->GameSettings.bIsDebugEnabled_WeaponModule)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "USB_WeaponModule::FireOnce : Projectile spawned");
	}
}

void USB_WeaponModule::Debug(float DeltaTime)
{
	if (DataManager == nullptr)
		return;

	if (DataManager->GameSettings.bIsDebugEnabled_WeaponModule == false)
		return;

	const FString StringToPrint = this->GetName() + " // bWantsToFire = " + FString::FromInt(bWantsToFire) + " // bIsFiring = " + FString::FromInt(bIsFiring);

	/*if (TargetModule)
		StringToPrint = this->GetName() + " // TargetModule : " + TargetModule->GetName();
	else
		StringToPrint = this->GetName() + " // TargetModule : nullptr";*/

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Magenta, StringToPrint);
}
