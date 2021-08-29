// SBGame
#include "Battle/SB_BattleGameMode.h"
#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "Battle/SB_BattlePlayerController.h"
#include "Battle/SB_AIController.h"
#include "Vehicle/SB_Vehicle.h"
#include "SB_PlayerStart.h"
#include "SB_GameInstance.h"
// Plugins
#include "RZ_UIManager.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "SB_GameSettings.h"

ASB_BattleGameMode::ASB_BattleGameMode()
{
	GameStateClass = ASB_GameState::StaticClass();
	PlayerStateClass = ASB_PlayerState::StaticClass();
	PlayerControllerClass = ASB_BattlePlayerController::StaticClass();
	HUDClass = ARZ_UIManager::StaticClass();
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;

	PrimaryActorTick.bCanEverTick = true;
}

void ASB_BattleGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld() == nullptr || GetWorld()->IsGameWorld() == false)
		return;

	for (TActorIterator<ASB_PlayerStart> PlayerStart(GetWorld()); PlayerStart; ++PlayerStart)
	{
		PlayerStarts.Add(*PlayerStart);
	}
	
	GState->GameType = ESB_GameType::Battle;
}

void ASB_BattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Spawn default bots.
	for (uint8 TeamID = 0; TeamID < GInstance->GameSettings->DefaultBotCountByTeam.Num(); TeamID++)
	{
		if (GState->TeamsData.IsValidIndex(TeamID))
		{
			for (uint8 Index = 0; Index < GInstance->GameSettings->DefaultBotCountByTeam[TeamID]; Index++)
			{
				SpawnAIController(TeamID);
			}
		}
	}
}

void ASB_BattleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASB_BattleGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	PlayerControllers.Add(Cast<ASB_BattlePlayerController>(NewPlayerController));
}

ASB_PlayerStart* ASB_BattleGameMode::GetAvailableShipStart(uint8 TeamID)
{
	for (ASB_PlayerStart* ShipStart : PlayerStarts)
	{
		if (ShipStart->GetIsEnabled() == true &&
			ShipStart->GetIsAvailable() == true &&
			ShipStart->GetTeamID() == TeamID)
		{
			return ShipStart;
		}
	}

	return nullptr;
}

void ASB_BattleGameMode::SpawnAIController(uint8 TeamID)
{
	if (GState->TeamsData.IsValidIndex(TeamID) == false ||
		GState->TeamsData[TeamID].PlayerList.Num() == GState->TeamsData[TeamID].MaxPlayers)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParameters;

	ASB_AIController* const NewAIController = GetWorld()->SpawnActorDeferred<ASB_AIController>(GInstance->AISettings.AIControllerClass, FTransform(), this);
	if (NewAIController)
	{
		NewAIController->bWantsPlayerState = true;
		UGameplayStatics::FinishSpawningActor(NewAIController, FTransform());
		
		ASB_PlayerState* const PlayerState = Cast<ASB_PlayerState>(NewAIController->PlayerState);
		if (PlayerState)
		{
			PlayerState->SetName(false);
		}

		AIControllers.Add(NewAIController);
		GState->AddPlayerToTeam(NewAIController, TeamID);
		
		OnAIControllerSpawned(NewAIController);
	}
}

void ASB_BattleGameMode::RemoveAIController(uint8 TeamID)
{
}

void ASB_BattleGameMode::OnAIControllerSpawned(ASB_AIController* const NewAIController)
{
}

void ASB_BattleGameMode::RegisterVehicle(AActor* const VehicleToRegister)
{
	ASB_Vehicle* const Vehicle = Cast<ASB_Vehicle>(VehicleToRegister);
	if (Vehicle)
	{
		SpawnedVehicles.Add(Vehicle);
	}
}

void ASB_BattleGameMode::UnregisterVehicle(ASB_Vehicle* const VehicleToUnregister, AController* OwningController)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "ASB_BattleGameMode::UnregisterVehicle");
	
	ASB_Vehicle* const Vehicle = Cast<ASB_Vehicle>(VehicleToUnregister);
	if (Vehicle)
	{
		SpawnedVehicles.Remove(Vehicle);
	}
}
