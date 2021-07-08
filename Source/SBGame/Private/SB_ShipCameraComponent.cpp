#include "SB_ShipCameraComponent.h"
//
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

USB_ShipCameraComponent::USB_ShipCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ShipCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraArmCT = GetOwner()->FindComponentByClass<USpringArmComponent>();
	CameraCT = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (CameraArmCT == nullptr || CameraCT == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("USB_ShipCameraComponent::BeginPlay - Owner missing CameraArmCT or CameraCT"));
		return;
	}

	CameraArmCT->SetRelativeLocation(FVector(0.0f, 0.0f, 5000.0f));
	CameraArmCT->TargetArmLength = DEFAULTARMLENGTH;
	TargetArmLength = DEFAULTARMLENGTH;
	CameraArmCT->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	TargetArmRotation = FRotator(-45.0f, 0.0f, 0.0f);
}

void USB_ShipCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CameraArmCT->TargetArmLength != TargetArmLength)
	{
		CameraArmCT->TargetArmLength = FMath::Lerp(CameraArmCT->TargetArmLength, TargetArmLength, ARMLENGTHLERPSPEED * DeltaTime);
	}

	if (CameraArmCT->GetComponentRotation() != TargetArmRotation)
	{
		CameraArmCT->SetWorldRotation(TargetArmRotation);
	}
}

void USB_ShipCameraComponent::AddPitchInput(float AxisValue)
{
	if (AxisValue == 0)
		return;

	if (CameraArmCT)
	{
		const float PitchValueToAdd = AxisValue * GetWorld()->GetDeltaSeconds() * 50.0f;

		if (AxisValue > 0 && CameraArmCT->GetRelativeRotation().Pitch + PitchValueToAdd >= MAXPITCH)
			return;

		if (AxisValue < 0 && CameraArmCT->GetRelativeRotation().Pitch + PitchValueToAdd <= MINPITCH)
			return;

		TargetArmRotation.Add(PitchValueToAdd, 0.0f, 0.0f);
		//CameraArmCT->AddRelativeRotation(FRotator(PitchValueToAdd, 0.0f, 0.0f));
	}
}

void USB_ShipCameraComponent::AddYawInput(float AxisValue)
{
	if (CameraArmCT)
	{
		const float YawValueToAdd = AxisValue * GetWorld()->GetDeltaSeconds() * 50.0f;
		TargetArmRotation.Add(0.0f, YawValueToAdd, 0.0f);

		//CameraArmCT->AddRelativeRotation(FRotator(0.0f, , 0.0f));
	}
}

void USB_ShipCameraComponent::ZoomIn()
{
	if (CameraArmCT)
	{
		TargetArmLength -= ARMLENGTHSTEP;
	}
}

void USB_ShipCameraComponent::ZoomOut()
{
	if (CameraArmCT)
	{
		TargetArmLength += ARMLENGTHSTEP;
	}
}

