#include "Battle/SB_FSGameMode.h"
#include "Battle/SB_BattlePlayerController.h"
#include "Battle/SB_AIController.h"
#include "Vehicle/SB_Vehicle.h"
#include "SB_PlayerStart.h"
#include "SB_GameInstance.h"
#include "SB_GameState.h"
#include "SB_GameSettings.h"
//
#include "TimerManager.h"

ASB_FSGameMode::ASB_FSGameMode()
{
}

void ASB_FSGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GState->GamePhase = ESB_GamePhase::Playing;
}

void ASB_FSGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASB_FSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleFreeRespawn();
}

void ASB_FSGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	QueryFreeRespawn(NewPlayerController);
}

void ASB_FSGameMode::OnAIControllerSpawned(ASB_AIController* const NewAIController)
{
	Super::OnAIControllerSpawned(NewAIController);

	QueryFreeRespawn(NewAIController);
}

void ASB_FSGameMode::QueryFreeRespawnWithDelay(AController* const NewController, float Delay)
{
	FTimerDelegate FreeRespawnTimerDelegate;
	FreeRespawnTimerDelegate.BindUFunction(this, FName("QueryFreeRespawn"), NewController);
	FTimerHandle FreeRespawnTimer;

	GetWorld()->GetTimerManager().SetTimer(
		FreeRespawnTimer,
		FreeRespawnTimerDelegate,
		GInstance->GameSettings->FreeRespawnTimer,
		false,
		GInstance->GameSettings->FreeRespawnTimer
	);
}

void ASB_FSGameMode::QueryFreeRespawn(AController* const NewController)
{
	if (ReadyToSpawnControllers.Contains(NewController) == false)
	{
		ReadyToSpawnControllers.Add(NewController);

		if (GInstance->DebugSettings.bIsDebugEnabled_GameMode)
			UE_LOG(LogTemp, Display, TEXT("ASB_FSGameMode::QueryRespawn - %s added to the spawn queue."), *NewController->GetName());
	}
}

void ASB_FSGameMode::HandleFreeRespawn()
{
	if (ReadyToSpawnControllers.Num() == 0)
		return;

	ISB_ControllerInterface* const ControllerInterface = Cast<ISB_ControllerInterface>(ReadyToSpawnControllers[0]);
	if (ControllerInterface)
	{
		ASB_PlayerStart* const ShipStart = GetAvailableShipStart(ControllerInterface->GetTeamID());
		if (ShipStart)
		{
			ASB_Vehicle* const NewVehicle = ControllerInterface->SpawnAndPossessVehicle(ShipStart->GetStartTransform());
			if (NewVehicle)
			{
				ReadyToSpawnControllers.RemoveAt(0);
				RegisterVehicle(NewVehicle);
				//NewVehicle->OnVehicleDestroyed.AddDynamic(this, &ASB_FSGameMode::UnregisterVehicle);
			}
		}
	}
}

void ASB_FSGameMode::UnregisterVehicle(ASB_Vehicle* const VehicleToUnregister, AController* OwningController)
{
	if (GInstance->DebugSettings.bIsDebugEnabled_GameMode)
		UE_LOG(LogTemp, Display, TEXT("ASB_FSGameMode::UnregisterVehicle - respawn timer set 0."));
	
	Super::UnregisterVehicle(VehicleToUnregister, OwningController);

	if (OwningController)
	{
		FTimerDelegate FreeRespawnTimerDelegate;
		FreeRespawnTimerDelegate.BindUFunction(this, FName("QueryFreeRespawn"), OwningController);
		FTimerHandle FreeRespawnTimer;

		GetWorld()->GetTimerManager().SetTimer(
			FreeRespawnTimer,
			FreeRespawnTimerDelegate,
			GInstance->GameSettings->FreeRespawnTimer,
			false,
			GInstance->GameSettings->FreeRespawnTimer
		);

		if (GInstance->DebugSettings.bIsDebugEnabled_GameMode)
			UE_LOG(LogTemp, Display, TEXT("ASB_FSGameMode::UnregisterVehicle - %s respawn timer set."), *OwningController->GetName());
	}
}
