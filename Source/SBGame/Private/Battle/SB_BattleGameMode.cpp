// SBGame
#include "Battle/SB_BattleGameMode.h"
#include "Battle/SB_GameState.h"
#include "Battle/SB_PlayerState.h"
#include "Battle/SB_BattlePlayerController.h"
#include "Battle/SB_AIController.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_ShipStart.h"
#include "SB_GameInstance.h"
// Plugins
#include "RZ_UIManager.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"

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

	if (GetWorld()->IsGameWorld() == false)
		return;

	GInstance = Cast<USB_GameInstance>(GetGameInstance());

	//

	for (TActorIterator<ASB_ShipStart> NewShipStart(GetWorld()); NewShipStart; ++NewShipStart)
	{
		ShipStarts.Add(*NewShipStart);
	}

	if (ShipStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_BaseGameMode::PostInitializeComponents - Missing ShipStart."));
	}
}

void ASB_BattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (uint8 Index = 0; Index < GInstance->AISettings.DefaultBotNum_Team1; Index++)
	{
		SpawnAIController(1);
	}

	for (uint8 Index = 0; Index < GInstance->AISettings.DefaultBotNum_Team2; Index++)
	{
		SpawnAIController(2);
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

ASB_ShipStart* ASB_BattleGameMode::GetAvailableShipStart(uint8 TeamID)
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

void ASB_BattleGameMode::SpawnAIController(uint8 TeamID)
{
	FActorSpawnParameters SpawnParameters;

	ASB_AIController* const NewAIController = GetWorld()->SpawnActorDeferred<ASB_AIController>(GInstance->AISettings.AIControllerClass, FTransform(), this);
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

void ASB_BattleGameMode::UnregisterVehicle(AActor* const VehicleToUnregister)
{
	ASB_Vehicle* const Vehicle = Cast<ASB_Vehicle>(VehicleToUnregister);
	if (Vehicle)
	{
		SpawnedVehicles.Remove(Vehicle);
	}
}
