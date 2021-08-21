#include "SB_WeaponModule.h"
#include "SB_WeaponModuleAnimInstance.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_TargetPoint.h"
#include "SB_GameInstance.h"
//
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region +++++ Setup ...

USB_WeaponModule::USB_WeaponModule()
{
	TargetShip = nullptr;
	TargetPoint = nullptr;
	LerpedRotation = GetComponentRotation();
	
	bIsSelected = false;
	bWantsToFire = false;
}

void USB_WeaponModule::Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName)
{
	Super::Init(NewModuleSlotData, NewModuleRowName);
	
	WeaponModuleData = GInstance->GetWeaponModuleDataFromRow(NewModuleRowName);
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

	UpdateRotation();
}

#pragma endregion

#pragma region +++++ Targeting ...

void USB_WeaponModule::SetTargetShip(ASB_Vehicle* const NewTargetShip)
{
	TargetShip = NewTargetShip;
}

void USB_WeaponModule::UpdateTargetComponent()
{
	TargetPoint = nullptr;
	
	if (GetOwnerRole() < ROLE_Authority)
		return;
	
	if (TargetShip == nullptr)
		return;

	FHitResult HitResult;
	const FVector Start = GetSocketLocation("ViewSocket");
	FCollisionQueryParams TraceParams;
	//TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.AddIgnoredComponent(this);

	for (USB_TargetPoint* const ThisTargetPoint : TargetShip->GetTargetPoints())
	{
		const FVector End = ThisTargetPoint->GetComponentLocation();
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
		
		if (HitResult.Actor == TargetShip)
		{
			if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
				UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Green, 0.3f, 20.0f);
			
			TargetPoint = ThisTargetPoint;
			//bWantsToFire = true;
			return;
		}

		if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
		{
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Red, TARGETUPDATERATE - 0.2f, 20.0f);
		}
	}
}

#pragma endregion

#pragma region +++++ Rotation ...

void USB_WeaponModule::UpdateRotation()
{
	if (TargetPoint.IsValid())
	{
		LerpedRotation = FMath::Lerp(
			LerpedRotation, 
			UKismetMathLibrary::FindLookAtRotation(GetSocketLocation("ViewSocket"), TargetPoint->GetComponentLocation()), 
			WeaponModuleData->RotationRate
		);
	}
	else
	{
		LerpedRotation = FMath::Lerp(
			LerpedRotation, 
			GetOwner()->GetActorRotation(), 
			WeaponModuleData->RotationRate);
	}

	// Apply yaw rotation on component itself
	SetWorldRotation(FRotator(0.0f, LerpedRotation.Yaw - 90.0f, 0.0f));

	// Apply pitch rotation on AnimInstance
	if (WeaponAnimInstance)
	{
		WeaponAnimInstance->UpdateInstance(LerpedRotation);
	}
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