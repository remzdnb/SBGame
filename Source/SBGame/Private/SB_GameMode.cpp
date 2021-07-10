#include "SB_GameMode.h"
#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "SB_PlayerController.h"
#include "SB_AIController.h"
#include "SB_ShipStart.h"
#include "SB_UIManager.h"
#include "SB_DataManager.h"
//
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

ASB_GameMode::ASB_GameMode()
{
	PlayerControllerClass = ASB_PlayerController::StaticClass();
	GameStateClass = ASB_GameState::StaticClass();
	PlayerStateClass = ASB_PlayerState::StaticClass();
	HUDClass = ASB_UIManager::StaticClass();

	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;

	PrimaryActorTick.bCanEverTick = true;
}

void ASB_GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	if (DataManager == nullptr)
		UE_LOG(LogTemp, Error, TEXT("ARZ_BaseGameMode::PostInitializeComponents - Missing DataManager."));

	for (TActorIterator<ASB_ShipStart> ShipStart(GetWorld()); ShipStart; ++ShipStart)
	{
		ShipStartArray.Add(*ShipStart);
	}

	if (ShipStartArray.Num() == 0)
		UE_LOG(LogTemp, Error, TEXT("ARZ_BaseGameMode::PostInitializeComponents - Missing ShipStart."));
}

void ASB_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASB_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle respawn.
	if (RegisteredPlayerControllers.Num() != 0)
	{
		ASB_PlayerController* const SelectedController = Cast<ASB_PlayerController>(RegisteredPlayerControllers[0]);
		if (SelectedController)
		{
			ASB_ShipStart* const ShipStart = GetAvailableShipStart(1);
			if (ShipStart)
			{
				SelectedController->SpawnAndPossessShip(ShipStart->GetStartTransform());
				RegisteredPlayerControllers.RemoveAt(0);

				//if (DataManager->bIsDebugEnabled_GameMode)
					//UE_LOG(LogTemp, Warning, TEXT("ARZ_BaseGameMode::HandleFreeRespawn - SpawnAndPossess called on controller."));
			}
		}
	}
}

void ASB_GameMode::QueryRespawn(AController* const NewController)
{
	ASB_PlayerController* const PlayerController = Cast<ASB_PlayerController>(NewController);
	if (PlayerController)
	{
		if (RegisteredPlayerControllers.Contains(PlayerController) == false)
		{
			RegisteredPlayerControllers.Add(PlayerController);

			if (DataManager->GameSettings.bIsDebugEnabled_GameMode)
				UE_LOG(LogTemp, Warning, TEXT("ARZ_BaseGameMode::QueryRespawn - %s added to the spawn queue."), *NewController->GetName());
		}
	}
}

class ASB_ShipStart* ASB_GameMode::GetAvailableShipStart(uint8 TeamID)
{
	for (ASB_ShipStart* ShipStart : ShipStartArray)
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

/*ASB_Ship* ASB_GameMode::SpawnShip(const APlayerStart* const ShipStart, bool bIsAIControlled)
{
	ASB_Ship* NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(DataManager->Settings.PawnClass, ShipStart->GetTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (NewShip)
	{
		if (bIsAIControlled)
			NewShip->AIControllerClass = DataManager->Settings.AIControllerClass;

		UGameplayStatics::FinishSpawningActor(NewShip, ShipStart->GetTransform());

		if (bIsAIControlled)
			NewShip->SpawnDefaultController();

		return NewShip;
	}

	return nullptr;
}*/

