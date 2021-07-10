#include "SB_ShipCameraManager.h"
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
}

void USB_ShipCameraManager::BeginPlay()
{
	Super::BeginPlay();

	MainCameraArm = NewObject<USpringArmComponent>(GetOwner(), FName("MainCameraArm"));
	if (MainCameraArm)
	{
		MainCameraArm->SetupAttachment(GetOwner()->GetRootComponent());
		MainCameraArm->RegisterComponent();
		MainCameraArm->bDoCollisionTest = true;
		MainCameraArm->ProbeChannel = ECC_Camera;
		MainCameraArm->TargetArmLength = DEFAULTARMLENGTH;
		MainCameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 5000.0f));
		MainCameraArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	}

	MainCamera = NewObject<UCameraComponent>(GetOwner(), FName("MainCamera"));
	if (MainCamera)
	{
		MainCamera->SetupAttachment(MainCameraArm);
		MainCamera->RegisterComponent();
	}

	TargetArmLength = DEFAULTARMLENGTH;
	TargetArmRotation = FRotator(-45.0f, 0.0f, 0.0f);
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

		if (MainCameraArm->GetComponentRotation() != TargetArmRotation)
		{
			MainCameraArm->SetWorldRotation(TargetArmRotation);
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

void USB_ShipCameraManager::ZoomIn()
{
	if (MainCameraArm)
	{
		TargetArmLength -= ARMLENGTHSTEP;
	}
}

void USB_ShipCameraManager::ZoomOut()
{
	if (MainCameraArm)
	{
		TargetArmLength += ARMLENGTHSTEP;
	}
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
		bIsSniperMode = false;
	}
	else
	{
		MainCamera->SetActive(false);
		SniperCameras[ActiveSniperCameraID]->SetActive(true);
		bIsSniperMode = true;
	}
}

