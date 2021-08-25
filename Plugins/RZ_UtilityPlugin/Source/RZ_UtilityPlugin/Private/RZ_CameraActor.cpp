#include "RZ_CameraActor.h"
//
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ARZ_CameraActor::ARZ_CameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = RootScene;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(RootScene);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);

	//

	CAMode = ERZ_CameraActorMode::Free;
}

void ARZ_CameraActor::BeginPlay()
{
	Super::BeginPlay();

	SpringArm->TargetArmLength = CASettings.DefaultArmLength_Free;
	TargetArmLength = CASettings.DefaultArmLength_Free;
	SpringArm->SetWorldRotation(FRotator(CASettings.DefaultPitch, 0.0f, 0.0f));
	TargetArmRotation = FRotator(CASettings.DefaultPitch, 0.0f, 0.0f);
}

void ARZ_CameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (TargetActor == nullptr && TargetComponent == nullptr)
	{
		if (GetActorLocation() != TargetLocation)
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLocation, LERPSPEED * DeltaTime));
		}
	}
	
	if (SpringArm->GetComponentRotation() != TargetArmRotation)
	{
		SpringArm->SetWorldRotation(FMath::Lerp(SpringArm->GetComponentRotation(), TargetArmRotation,
		                                        LERPSPEED * DeltaTime));
	}

	if (SpringArm->TargetArmLength != TargetArmLength)
	{
		SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, TargetArmLength, LERPSPEED * DeltaTime);
	}*/
}

void ARZ_CameraActor::AddPitchInput(float AxisValue)
{
	if (SpringArm == nullptr || AxisValue == 0)
		return;

	const float NewPitch = SpringArm->GetComponentRotation().Pitch + (AxisValue * GetWorld()->GetDeltaSeconds() * 50.0f);
	
	if (NewPitch > MAXPITCH || NewPitch < MINPITCH)
		return;

	SpringArm->SetWorldRotation(FRotator(
		NewPitch,
		SpringArm->GetComponentRotation().Yaw,
		SpringArm->GetComponentRotation().Roll
	));
	
	TargetArmRotation = FRotator(
		NewPitch,
		TargetArmRotation.Yaw,
		TargetArmRotation.Roll
	);
}

void ARZ_CameraActor::AddYawInput(float AxisValue)
{
	if (SpringArm == nullptr || AxisValue == 0)
		return;

	const float NewYaw = SpringArm->GetComponentRotation().Yaw + (AxisValue * GetWorld()->GetDeltaSeconds() * 50.0f);

	SpringArm->SetWorldRotation(FRotator(
		SpringArm->GetComponentRotation().Pitch,
		NewYaw,
		SpringArm->GetComponentRotation().Roll));

	TargetArmRotation = FRotator(
		TargetArmRotation.Pitch,
		NewYaw,
		TargetArmRotation.Roll
	);
}

void ARZ_CameraActor::AddZoomInput(bool bZoomIn)
{
	TargetArmLength = bZoomIn
		                  ? FMath::Clamp(TargetArmLength - ARMLENGTHSTEP, MINARMLENGTH, MAXARMLENGTH)
		                  : FMath::Clamp(TargetArmLength + ARMLENGTHSTEP, MINARMLENGTH, MAXARMLENGTH);
}

void ARZ_CameraActor::SetNewTargetActor(AActor* NewTargetActor, FVector RelativeOffset)
{
	TargetComponent = nullptr;
	
	TargetActor = NewTargetActor;
	AttachToActor(TargetActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SpringArm->SetRelativeLocation(RelativeOffset);
}

void ARZ_CameraActor::SetNewTargetComponent(USceneComponent* NewTargetComponent)
{
	TargetActor = nullptr;
	TargetComponent = NewTargetComponent;
}

void ARZ_CameraActor::SetNewLocation(const FVector& NewLocation, bool bWithLerp)
{
	TargetLocation = NewLocation;

	if (bWithLerp == false)
		SetActorLocation(NewLocation);
}

void ARZ_CameraActor::SetNewRotation(const FRotator& NewRotation, bool bWithLerp)
{
	TargetArmRotation = NewRotation;

	if (bWithLerp == false)
		SpringArm->SetWorldRotation(NewRotation);
}

void ARZ_CameraActor::SetNewArmLength(float NewArmLength, bool bWithLerp)
{
	TargetArmLength = NewArmLength;

	if (bWithLerp == false)
		SpringArm->TargetArmLength = NewArmLength;
}
