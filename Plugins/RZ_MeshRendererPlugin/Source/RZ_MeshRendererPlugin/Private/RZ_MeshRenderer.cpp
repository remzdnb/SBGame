//
#include "RZ_MeshRenderer.h"
// Engine
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ARZ_MeshRenderer::ARZ_MeshRenderer()
{
	SceneCT = CreateDefaultSubobject<USceneComponent>(FName("SceneCT"));
	RootComponent = SceneCT;

	CameraArmCT = CreateDefaultSubobject<USpringArmComponent>(FName("CameraArmCT"));
	CameraArmCT->SetupAttachment(RootComponent);
	CameraArmCT->TargetArmLength = 70.0f;
	CameraArmCT->bDoCollisionTest = false;

	CaptureCT = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("CaptureCT"));
	CaptureCT->SetupAttachment(CameraArmCT);

	CameraCT = CreateDefaultSubobject<UCameraComponent>(FName("CameraCT"));
	CameraCT->SetupAttachment(CameraArmCT);
	CameraCT->SetProjectionMode(ECameraProjectionMode::Orthographic);

	ArrowCT = CreateDefaultSubobject<UArrowComponent>(FName("ArrowCT"));
	ArrowCT->SetupAttachment(RootComponent);
	ArrowCT->SetRelativeLocation(FVector(500.0f, 0.0f, 0.0f));

	PrimaryActorTick.bCanEverTick = true;

	CameraArmCT->TargetArmLength = 100.0f;
	CameraCT->OrthoWidth = 1000.0f;
}

void ARZ_MeshRenderer::BeginPlay()
{
	Super::BeginPlay();

	if (SkeletalMesh)
	{
		SkeletalMesh->SetActorTransform(ArrowCT->GetComponentTransform());
	}

	if (StaticMesh)
	{
		StaticMesh->SetActorTransform(ArrowCT->GetComponentTransform());
	}
}

void ARZ_MeshRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARZ_MeshRenderer::SetArmLength(float NewArmLength)
{
	CameraArmCT->TargetArmLength = NewArmLength;
}

void ARZ_MeshRenderer::SetCameraOrthoWidth(float NewOrthoWidth)
{
	CameraCT->SetOrthoWidth(NewOrthoWidth);
}

void ARZ_MeshRenderer::SetActorPitch(float NewPitch)
{
	if (SkeletalMesh)
		SkeletalMesh->SetActorRotation(FRotator(NewPitch, SkeletalMesh->GetActorRotation().Yaw, SkeletalMesh->GetActorRotation().Roll));
	if (StaticMesh)
		StaticMesh->SetActorRotation(FRotator(NewPitch, StaticMesh->GetActorRotation().Yaw, StaticMesh->GetActorRotation().Roll));
}

void ARZ_MeshRenderer::SetActorYaw(float NewYaw)
{
	if (SkeletalMesh)
		SkeletalMesh->SetActorRotation(FRotator(SkeletalMesh->GetActorRotation().Pitch, NewYaw, SkeletalMesh->GetActorRotation().Roll));
	if (StaticMesh)
		StaticMesh->SetActorRotation(FRotator(StaticMesh->GetActorRotation().Pitch, NewYaw, StaticMesh->GetActorRotation().Roll));
}

void ARZ_MeshRenderer::SetActorRoll(float NewRoll)
{
	if (SkeletalMesh)
		SkeletalMesh->SetActorRotation(FRotator(SkeletalMesh->GetActorRotation().Pitch, SkeletalMesh->GetActorRotation().Yaw, NewRoll));
	if (StaticMesh)
		StaticMesh->SetActorRotation(FRotator(StaticMesh->GetActorRotation().Pitch, StaticMesh->GetActorRotation().Yaw, NewRoll));
}

void ARZ_MeshRenderer::AddActorYOffset(float NewYOffset)
{
	if (SkeletalMesh)
		SkeletalMesh->AddActorWorldOffset(FVector(0.0f, NewYOffset, 0.0f));
	if (StaticMesh)
		StaticMesh->AddActorWorldOffset(FVector(0.0f, NewYOffset, 0.0f));
}

void ARZ_MeshRenderer::AddActorZOffset(float NewZOffset)
{
	if (SkeletalMesh)
		if (SkeletalMesh)
			SkeletalMesh->AddActorWorldOffset(FVector(0.0f, 0.0f, NewZOffset));
	if (StaticMesh)
		StaticMesh->AddActorWorldOffset(FVector(0.0f, 0.0f, NewZOffset));
}
