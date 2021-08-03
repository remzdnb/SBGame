#include "Battle/SB_AIController.h"
#include "SB_PlayerController.h"
#include "SB_GameMode.h"
#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "SB_DataManager.h"
//
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "EngineUtils.h"

ASB_AIController::ASB_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASB_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	GMode = Cast<ASB_GameMode>(GetWorld()->GetAuthGameMode());
	GState = Cast<ASB_GameState>(GetWorld()->GetGameState());
	PState = Cast<ASB_PlayerState>(PlayerState);
}

void ASB_AIController::BeginPlay()
{
	Super::BeginPlay();

	GMode->QueryRespawn(this);

	GetWorldTimerManager().SetTimer(DetectionUpdateTimer, this, &ASB_AIController::UpdateDetection, DataManager->AISettings.DetectionUpdateRate, true, 0.0f);
}

void ASB_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DataManager == nullptr || OwnedShip == nullptr)
		return;

	UpdateMovement();

	// omg
	if (OwnedShip.IsValid())
	{
		if (OwnedShip->GetShipMovement())
		{
			//float LerpedYaw = FMath::Lerp(GetControlRotation().Yaw, OwnedShip->GetShipMovement()->GetTargetRotationYaw(), DataManager->ShipSettings.TurnInertia);
			OwnedShip->SetActorRotation(FRotator(0.0f, OwnedShip->GetShipMovement()->GetTargetRotationYaw(), 0.0f));
			//SetControlRotation(FRotator(0.0f, LerpedYaw, 0.0f));
			//SetFocus / SetFocalpoint?
		}
	}
}

void ASB_AIController::SpawnAndPossessShip(const FTransform& SpawnTransform)
{
	if (GetPawn() != nullptr)
		return;

	ASB_Ship* const NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(DataManager->GameSettings.ShipClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (NewShip)
	{
		UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
		OnPossess(NewShip);
	}
}

void ASB_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnedShip = Cast<ASB_Ship>(InPawn);
}

void ASB_AIController::UpdateDetection()
{
	if (DataManager == nullptr || OwnedShip == nullptr)
		return;

	CollisionActor_Right = nullptr;
	CollisionActor_Left = nullptr;

	UpdateCollisionActor(true);
	UpdateCollisionActor(false);

	UpdateTargetShip();
}

void ASB_AIController::UpdateCollisionActor(bool bIsRightActor)
{
	const FVector StartLocation = OwnedShip->GetActorLocation();
	const FRotator EndRotation = bIsRightActor ? OwnedShip->GetActorRotation() + FRotator(0.0f, 15.0f, 0.0f) : OwnedShip->GetActorRotation() + FRotator(0.0f, -15.0f, 0.0f);
	const FVector EndLocation = StartLocation + (EndRotation.Vector() * DataManager->AISettings.CollisionDetectionRange);
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(DataManager->AISettings.CollisionDetectionSphereRadius);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnedShip.Get());
	TArray<FHitResult> Hits;
	GetWorld()->SweepMultiByChannel(Hits, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, CollisionSphere);
	for (FHitResult& Hit : Hits)
	{
		if (Hit.Actor.IsValid() && Hit.Actor != GetPawn())
		{
			bIsRightActor ? CollisionActor_Right = Hit.Actor.Get() : CollisionActor_Left = Hit.Actor.Get();

			if (DataManager->GameSettings.bIsDebugEnabled_AI)
			{
				FString ActorRight = bIsRightActor ? "Right : " : "Left : ";
				GEngine->AddOnScreenDebugMessage(-1, DataManager->AISettings.DetectionUpdateRate, FColor::Purple, *(ActorRight + Hit.Actor->GetName()));
				UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.Location, DataManager->AISettings.CollisionDetectionSphereRadius, 10, FColor::Red, DataManager->AISettings.DetectionUpdateRate + 0.05f, 50.0f);
			}

			break;
		}
	}

	if (DataManager->GameSettings.bIsDebugEnabled_AI)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), EndLocation, DataManager->AISettings.CollisionDetectionSphereRadius, 10, FColor::Green, DataManager->AISettings.DetectionUpdateRate + 0.05f, 50.0f);
	}
}

void ASB_AIController::UpdateMovement()
{
	if (CollisionActor_Right.IsValid() == false && CollisionActor_Left.IsValid() == false)
	{
		OwnedShip->GetShipMovement()->MoveForward(1.0f);
	}
	else if (CollisionActor_Right.IsValid() == true && CollisionActor_Left.IsValid() == true)
	{
		OwnedShip->GetShipMovement()->TurnRight(1.0f);
	}
	else if (CollisionActor_Right.IsValid() == true && CollisionActor_Left.IsValid() == false)
	{
		OwnedShip->GetShipMovement()->MoveForward(1.0f);
		OwnedShip->GetShipMovement()->TurnRight(-1.0f);
	}
	else if (CollisionActor_Right.IsValid() == false && CollisionActor_Left.IsValid() == true)
	{
		OwnedShip->GetShipMovement()->MoveForward(1.0f);
		OwnedShip->GetShipMovement()->TurnRight(1.0f);
	}
}

void ASB_AIController::UpdateTargetShip()
{
	TargetShip = nullptr;

	TArray<ASB_Ship*> TargetShips;
	//ASB_Ship* ClosestTargetShip;
	float ClosestTargetDistance = 999999999.0f;

	for (auto& PlayerController : GMode->GetPlayerControllers())
	{
		if (PlayerController->GetPlayerState()->GetTeam() != PState->GetTeam())
		{
			ASB_Ship* const LocalShip = PlayerController->GetOwnedShip();
			if (LocalShip)
			{
				TargetShips.Add(LocalShip);
			}
		}
	}

	for (auto& AIController : GMode->GetAIControllers())
	{
		if (AIController->GetPlayerState()->GetTeam() != PState->GetTeam())
		{
			ASB_Ship* const LocalShip = AIController->GetOwnedShip().Get();
			if (LocalShip)
			{
				TargetShips.Add(LocalShip);
			}
		}
	}

	for (auto& LocalTargetShip : TargetShips)
	{
		const FVector Start = OwnedShip->GetActorLocation();
		const FVector End = LocalTargetShip->GetActorLocation();
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(OwnedShip.Get());
		TArray<FHitResult> Hits;

		GetWorld()->LineTraceMultiByChannel(Hits, Start, End, ECC_Visibility, TraceParams);
		for (FHitResult& Hit : Hits)
		{
			if (Hit.Actor.IsValid() && Hit.Actor == LocalTargetShip)
			{
				TargetShip = LocalTargetShip;
				
				if (DataManager->GameSettings.bIsDebugEnabled_AI)
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Green, DataManager->AISettings.DetectionUpdateRate, 50.0f);

				return;
			}
			else
			{
				if (DataManager->GameSettings.bIsDebugEnabled_AI)
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, DataManager->AISettings.DetectionUpdateRate, 50.0f);

				break;
			}
		}
	}
}