#include "Battle/SB_SpectatorPawn.h"
//
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ASB_SpectatorPawn::ASB_SpectatorPawn()
{
	CameraArmCT = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArmCT"));
	CameraArmCT->SetupAttachment(RootComponent);
	CameraArmCT->TargetArmLength = 10000.0f;
	CameraArmCT->bUsePawnControlRotation = true;
	CameraArmCT->bDoCollisionTest = false;

	CameraCT = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCT"));
	CameraCT->SetupAttachment(CameraArmCT);
	CameraCT->bUsePawnControlRotation = false;
	CameraCT->bAutoActivate = true;

	//

	DefaultMovementSpeed = 10000.0f;
	QuickMovementSpeed = 30000.0f;
	TargetArmLength = CameraArmCT->TargetArmLength;
	ZoomStep = 1000.0f;
	ZoomSpeed = 15.0f;

	//

	GetCollisionComponent()->SetCollisionProfileName("IgnoreAll");
	GetMeshComponent()->SetCollisionProfileName("IgnoreAll");
	GetMeshComponent()->bOwnerNoSee = false;
	GetMeshComponent()->bCastDynamicShadow = false;
	bAddDefaultMovementBindings = false;

	FloatingPawnMovement = Cast<UFloatingPawnMovement>(MovementComponent);
	if (FloatingPawnMovement)
	{
		FloatingPawnMovement->MaxSpeed = DefaultMovementSpeed;
		FloatingPawnMovement->Acceleration = 999999.0f;
		FloatingPawnMovement->Deceleration = 999999.0f;
	}

	//

	AIControllerClass = nullptr;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	SetReplicates(false);
	SetReplicateMovement(false);
}

void ASB_SpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ASB_SpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraArmCT->TargetArmLength != TargetArmLength)
	{
		CameraArmCT->TargetArmLength = FMath::Lerp(CameraArmCT->TargetArmLength, TargetArmLength, ZoomSpeed * DeltaTime);
	}
}

void ASB_SpectatorPawn::MoveForward(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		if (Controller)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, AxisValue);
		}
	}
}

void ASB_SpectatorPawn::MoveRight(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		if (Controller)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, AxisValue);
		}
	}
}

void ASB_SpectatorPawn::Zoom(float AxisValue)
{
	if (AxisValue > 0)
	{
		TargetArmLength -= ZoomStep;
	}

	if (AxisValue < 0)
	{
		TargetArmLength += ZoomStep;
	}
}

void ASB_SpectatorPawn::ToggleQuickMovement(bool bIsEnabled)
{
	if (FloatingPawnMovement == nullptr)
		return;

	if (bIsEnabled)
		FloatingPawnMovement->MaxSpeed = QuickMovementSpeed;
	else
		FloatingPawnMovement->MaxSpeed = DefaultMovementSpeed;
}
