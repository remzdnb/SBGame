#include "SB_WeaponModule.h"
#include "SB_WeaponModuleAnimInstance.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_TargetPoint.h"
#include "SB_GameInstance.h"
#include "Battle/SB_PlayerState.h"
//
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region +++++ Setup ...

USB_WeaponModule::USB_WeaponModule()
{
	bIsAIEnabled = false;
	bWantsToFire = false;
}

void USB_WeaponModule::Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName)
{
	Super::Init(NewModuleSlotData, NewModuleRowName);
	
	WeaponModuleData = GInstance->GetWeaponModuleDataFromRow(NewModuleRowName);
}

void USB_WeaponModule::BeginPlay()
{
	Super::BeginPlay();

	DefaultRotation = CurrentRotation = GetRelativeRotation();

	WeaponAnimInstance = Cast<USB_WeaponModuleAnimInstance>(GetAnimInstance());

	GetWorld()->GetTimerManager().SetTimer(TargetUpdateTimer, this, &USB_WeaponModule::UpdateTargetComponent, TARGETUPDATERATE, true, 0.0f);
}

#pragma endregion

#pragma region +++++ Main ...

void USB_WeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WeaponModuleData == nullptr)
		return;

	UpdateRotation(DeltaTime);
	UpdateWeaponTrace();
}

#pragma endregion

#pragma region +++++ Targeting ...

void USB_WeaponModule::SetTargets(TArray<ASB_Vehicle*> NewTargets)
{
	TargetVehicles = NewTargets;
}

void USB_WeaponModule::UpdateTargetComponent()
{
	TargetPoint = nullptr;
	
	if (GetOwnerRole() < ROLE_Authority)
		return;
	
	if (TargetVehicles.Num() == 0)
		return;

	FHitResult HitResult;
	const FVector Start = GetSocketLocation("ViewSocket");
	FCollisionQueryParams TraceParams;
	//TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.AddIgnoredComponent(this);

	for (auto& TargetVehicle : TargetVehicles)
	{
		for (USB_TargetPoint* const ThisTargetPoint : TargetVehicle->GetTargetPoints())
		{
			const FVector End =
				Start +
				UKismetMathLibrary::FindLookAtRotation(Start, ThisTargetPoint->GetComponentLocation()).Vector() *
				WeaponModuleData->Range;
			
			GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
			if (HitResult.Actor == TargetVehicle)
			{
				if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Green, 0.2f, 20.0f);
			
				TargetPoint = ThisTargetPoint;
				return;
			}

			if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
			{
				UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Red, 0.2f, 20.0f);
			}
		}
	}
}

void USB_WeaponModule::UpdateWeaponTrace()
{
	if (GetOwnerRole() < ROLE_Authority)
		return;
	
	const FVector Start = GetSocketLocation("ViewSocket");
	const FVector End =
		GetSocketLocation("ViewSocket") +
		GetSocketRotation("ViewSocket").Vector() *
		WeaponModuleData->Range;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredComponent(this);

	GetWorld()->LineTraceSingleByChannel(WeaponTraceResult, Start, End, ECC_Visibility, TraceParams);
	ASB_Vehicle* HitVehicle = Cast<ASB_Vehicle>(WeaponTraceResult.Actor);
	if (HitVehicle)
	{
		const ASB_PlayerState* const SelfPlayerState = Cast<ASB_PlayerState>(Cast<APawn>(GetOwner())->GetPlayerState());
		const ASB_PlayerState* const TargetPlayerState = Cast<ASB_PlayerState>(HitVehicle->GetPlayerState());
		if (SelfPlayerState && TargetPlayerState)
		{
			if (SelfPlayerState->GetTeam() != TargetPlayerState->GetTeam())
			{
				if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
				{
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor(255, 255, 255, 1),
					                                    GetWorld()->GetDeltaSeconds() + 0.01f, 30.0f);
				}

				return;
			}
		}
	}

	if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor(64, 64, 64, 1),
		                                    GetWorld()->GetDeltaSeconds() + 0.01f, 30.0f);
	}
}

#pragma endregion

#pragma region +++++ Rotation ...

void USB_WeaponModule::UpdateRotation(float DeltaTime)
{
	if (TargetPoint.IsValid())
	{
		const FRotator TargetWorldRotation = UKismetMathLibrary::FindLookAtRotation(
			GetSocketLocation("ViewSocket"), TargetPoint->GetComponentLocation());
		
		const FRotator TargetRelativeRotation = UKismetMathLibrary::InverseTransformRotation(
			GetOwner()->GetTransform(), TargetWorldRotation);

		CurrentRotation = FMath::RInterpConstantTo(
			GetRelativeRotation(),
			FRotator(TargetRelativeRotation.Pitch, TargetRelativeRotation.Yaw - 90.0f, TargetRelativeRotation.Roll),
			DeltaTime,
			WeaponModuleData->RotationRate
		);
		
		if (WeaponAnimInstance)
		{
			WeaponAnimInstance->UpdateInstance(TargetWorldRotation);
		}
	}
	else
	{
		CurrentRotation = FMath::RInterpConstantTo(
			GetRelativeRotation(),
			DefaultRotation,
			DeltaTime,
			WeaponModuleData->RotationRate / 2
		);

		if (WeaponAnimInstance)
		{
			WeaponAnimInstance->UpdateInstance(FRotator::ZeroRotator);
		}
	}

	// Apply yaw rotation on component itself
	SetRelativeRotation(FRotator(DefaultRotation.Pitch, CurrentRotation.Yaw, DefaultRotation.Roll));

	// Apply pitch rotation on AnimInstance

}

#pragma endregion

#pragma region +++++ Fire ...

void USB_WeaponModule::SetWantsToFire(bool bNewWantsToFire)
{
	bWantsToFire = bNewWantsToFire;
}

#pragma endregion

#pragma region +++++ Network ...

//

#pragma endregion