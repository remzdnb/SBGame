#include "SB_GameMode.h"
#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "SB_PlayerController.h"
#include "Battle/SB_AIController.h"
#include "Ship/SB_ShipStart.h"
#include "RZ_UIManager.h"
//
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"

ASB_GameMode::ASB_GameMode()
{
	GameStateClass = ASB_GameState::StaticClass();
	PlayerStateClass = ASB_PlayerState::StaticClass();
	HUDClass = ARZ_UIManager::StaticClass();
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;

	PrimaryActorTick.bCanEverTick = true;
}

void ASB_GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GInstance = Cast<USB_GameInstance>(GetGameInstance());

	for (TActorIterator<ASB_ShipStart> NewShipStart(GetWorld()); NewShipStart; ++NewShipStart)
	{
		ShipStarts.Add(*NewShipStart);
	}

	if (ShipStarts.Num() == 0)
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
		ASB_PlayerController* const SelectedPlayerController = RegisteredPlayerControllers[0].Get();
		if (SelectedPlayerController)
		{
			ASB_PlayerState* const PlayerState = Cast<ASB_PlayerState>(SelectedPlayerController->PlayerState);
			if (PlayerState)
			{
				ASB_ShipStart* const ShipStart = GetAvailableShipStart(PlayerState->GetTeam());
				if (ShipStart)
				{
					SelectedPlayerController->SpawnAndPossessShip(ShipStart->GetStartTransform());
					RegisteredPlayerControllers.RemoveAt(0);
				}
			}
		}
	}
	else
	{
		if (RegisteredAIControllers.Num() != 0)
		{
			ASB_AIController* const SelectedAIController = RegisteredAIControllers[0].Get();
			if (SelectedAIController)
			{
				ASB_PlayerState* const PlayerState = Cast<ASB_PlayerState>(SelectedAIController->PlayerState);
				if (PlayerState)
				{
					ASB_ShipStart* const ShipStart = GetAvailableShipStart(PlayerState->GetTeam());
					if (ShipStart)
					{
						SelectedAIController->SpawnAndPossessShip(ShipStart->GetStartTransform());
						RegisteredAIControllers.RemoveAt(0);
					}
				}
			}
		}
	}
}

void ASB_GameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	PlayerControllers.Add(Cast<ASB_PlayerController>(NewPlayerController));
}

void ASB_GameMode::QueryRespawn(AController* const NewController)
{
	ASB_PlayerController* const PlayerController = Cast<ASB_PlayerController>(NewController);
	if (PlayerController)
	{
		if (RegisteredPlayerControllers.Contains(PlayerController) == false)
		{
			RegisteredPlayerControllers.Add(PlayerController);

			if (GInstance->GameSettings.bIsDebugEnabled_GameMode)
				UE_LOG(LogTemp, Warning, TEXT("ARZ_BaseGameMode::QueryRespawn - Player : %s added to the spawn queue."), *NewController->GetName());
		}
	}
	else
	{
		ASB_AIController* const AIController = Cast<ASB_AIController>(NewController);
		if (AIController)
		{
			if (RegisteredAIControllers.Contains(AIController) == false)
			{
				RegisteredAIControllers.Add(AIController);

				if (GInstance->GameSettings.bIsDebugEnabled_GameMode)
					UE_LOG(LogTemp, Warning, TEXT("ARZ_BaseGameMode::QueryRespawn - AI : %s added to the spawn queue."), *NewController->GetName());
			}
		}
	}
}

ASB_ShipStart* ASB_GameMode::GetAvailableShipStart(uint8 TeamID)
{
	for (ASB_ShipStart* ShipStart : ShipStarts)
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

void ASB_GameMode::SpawnAIController(uint8 TeamID)
{
	FActorSpawnParameters SpawnParameters;

	ASB_AIController* const NewAIController = GetWorld()->SpawnActorDeferred<ASB_AIController>(GInstance->GameSettings.AIControllerClass, FTransform(), this);
	if (NewAIController)
	{
		NewAIController->bWantsPlayerState = true;
		UGameplayStatics::FinishSpawningActor(NewAIController, FTransform());

		AIControllers.Add(NewAIController);

		ASB_PlayerState* const PlayerState = Cast<ASB_PlayerState>(NewAIController->PlayerState);
		if (PlayerState)
		{
			PlayerState->SetName(false);
			PlayerState->SetTeamID(TeamID);
		}
	}
}

void ASB_GameMode::RemoveAIController(uint8 Team)
{
}