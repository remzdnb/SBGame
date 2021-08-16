#include "Ship/SB_ShipCameraManager.h"
#include "Ship/SB_Ship.h"
#include "Module/Weapon/SB_BaseWeaponModule.h"
//
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

USB_ShipCameraManager::USB_ShipCameraManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	//

	bIsSniperMode = false;
	bLerpRotation = false;
	ActiveSniperCameraID = 0;
}

void USB_ShipCameraManager::BeginPlay()
{
	Super::BeginPlay();

	OwningShip = Cast<ASB_Ship>(GetOwner());

	MainCameraArm = NewObject<USpringArmComponent>(GetOwner(), FName("MainCameraArm"));
	if (MainCameraArm)
	{
		MainCameraArm->SetupAttachment(OwningShip->GetRootComponent());
		MainCameraArm->RegisterComponent();
		MainCameraArm->bDoCollisionTest = false;
		MainCameraArm->ProbeChannel = ECC_Camera;
		MainCameraArm->TargetArmLength = DEFAULTARMLENGTH;
		MainCameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 5000.0f));
	}

	MainCamera = NewObject<UCameraComponent>(GetOwner(), FName("MainCamera"));
	if (MainCamera)
	{
		MainCamera->SetupAttachment(MainCameraArm);
		MainCamera->RegisterComponent();
		MainCamera->SetAutoActivate(true);
	}

	TargetArmLength = DEFAULTARMLENGTH;
	TargetArmLocation = FVector::ZeroVector;
	//TargetArmRotation = FRotator(-10.0f, GetOwner()->GetActorRotation().Yaw, 0.0f);
}

void USB_ShipCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSniperMode)
	{
		SniperCameras[ActiveSniperCameraID]->SetWorldRotation(TargetArmRotation);
	}
	else
	{
		if (MainCameraArm->TargetArmLength != TargetArmLength)
		{
			MainCameraArm->TargetArmLength = FMath::Lerp(MainCameraArm->TargetArmLength, TargetArmLength, ARMLENGTHLERPSPEED * DeltaTime);
		}

		if (MainCameraArm->GetRelativeLocation() != TargetArmLocation)
		{
			MainCameraArm->SetRelativeLocation(FMath::Lerp(MainCameraArm->GetRelativeLocation(), TargetArmLocation, ARMLENGTHLERPSPEED * DeltaTime));
		}

		if (MainCameraArm->GetComponentRotation() != TargetArmRotation)
		{
			MainCameraArm->SetWorldRotation(FMath::Lerp(MainCameraArm->GetComponentRotation(), TargetArmRotation, ARMLENGTHLERPSPEED * DeltaTime));
		}
	}
}

void USB_ShipCameraManager::AddPitchInput(float AxisValue)
{
	if (AxisValue == 0)
		return;

	if (MainCameraArm)
	{
		const float PitchValueToAdd = AxisValue * GetWorld()->GetDeltaSeconds() * 50.0f;

		if (AxisValue > 0 && MainCameraArm->GetRelativeRotation().Pitch + PitchValueToAdd >= MAXPITCH)
			return;

		if (AxisValue < 0 && MainCameraArm->GetRelativeRotation().Pitch + PitchValueToAdd <= MINPITCH)
			return;

		TargetArmRotation.Add(PitchValueToAdd, 0.0f, 0.0f);
	}
}

void USB_ShipCameraManager::AddYawInput(float AxisValue)
{
	if (MainCameraArm)
	{
		const float YawValueToAdd = AxisValue * GetWorld()->GetDeltaSeconds() * 50.0f;
		
		TargetArmRotation.Add(0.0f, YawValueToAdd, 0.0f);
	}
}

void USB_ShipCameraManager::Zoom(bool bZoomIn)
{
	if (bIsSniperMode)
	{
		
	}
	else
	{
		TargetArmLength = bZoomIn
			? FMath::Clamp(TargetArmLength - ARMLENGTHSTEP, MINARMLENGTH, MAXARMLENGTH)
			: FMath::Clamp(TargetArmLength + ARMLENGTHSTEP, MINARMLENGTH, MAXARMLENGTH);
	}
}

void USB_ShipCameraManager::SetArmRelativeLocation(const FVector& RelativeLocation)
{
	TargetArmLocation = RelativeLocation;
}

void USB_ShipCameraManager::AddSniperCamera(UCameraComponent* const NewWeaponCamera)
{
	SniperCameras.Add(NewWeaponCamera);
}

void USB_ShipCameraManager::SetActiveSniperCamera(uint8 NewActiveSniperCameraID)
{
	if (bIsSniperMode)
	{
		SniperCameras[ActiveSniperCameraID]->SetActive(false);
		SniperCameras[NewActiveSniperCameraID]->SetActive(true);
	}

	ActiveSniperCameraID = NewActiveSniperCameraID;
}

void USB_ShipCameraManager::ToggleSniperMode()
{
	if (bIsSniperMode)
	{
		SniperCameras[ActiveSniperCameraID]->SetActive(false);
		MainCamera->SetActive(true);
		//if (OwningShip->GetWeaponModules()[ActiveSniperCameraID])
			//OwningShip->GetWeaponModules()[ActiveSniperCameraID]->SetHiddenInGame(true);
		bIsSniperMode = false;
	}
	else
	{
		MainCamera->SetActive(false);
		SniperCameras[ActiveSniperCameraID]->SetActive(true);
		//if (OwningShip->GetWeaponModules()[ActiveSniperCameraID])
			OwningShip->GetWeaponModules()[ActiveSniperCameraID]->SetHiddenInGame(true);
		bIsSniperMode = true;
	}
}

void USB_ShipCameraManager::SetArmRotation(const FRotator& NewRotation, bool bShouldLerp)
{
	TargetArmRotation = NewRotation;

	if (bShouldLerp == false)
		MainCameraArm->SetWorldRotation(NewRotation);
}