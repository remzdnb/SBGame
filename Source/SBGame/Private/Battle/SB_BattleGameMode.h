#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SB_BattleGameMode.generated.h"

class USB_GameInstance;
class ASB_ShipStart;
class ASB_BattlePlayerController;
class ASB_AIController;
class ASB_Ship;

UCLASS()
class ASB_BattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASB_BattleGameMode();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayerController) override;

	//

	UFUNCTION()
	void RegisterShip(ASB_Ship* const ShipToRegister);

	UFUNCTION()
	void UnregisterShip(ASB_Ship* const ShipToUnregister);
	
	//

	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_BattlePlayerController>> const GetPlayerControllers() { return PlayerControllers; }
	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_AIController>> const GetAIControllers() { return AIControllers; }
	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_Ship>> const GetSpawnedShips() { return SpawnedShips; }

protected:

	USB_GameInstance* GInstance;

	TArray<ASB_ShipStart*> ShipStarts;
	TArray<TWeakObjectPtr<ASB_BattlePlayerController>> PlayerControllers;
	TArray<TWeakObjectPtr<ASB_AIController>> AIControllers;
	TArray<TWeakObjectPtr<ASB_Ship>> SpawnedShips;
	
	//

	UFUNCTION()
	ASB_ShipStart* GetAvailableShipStart(uint8 TeamID);

	UFUNCTION(Exec)
	void SpawnAIController(uint8 TeamID);

	UFUNCTION(Exec)
	void RemoveAIController(uint8 TeamID);

	UFUNCTION()
	virtual void OnAIControllerSpawned(ASB_AIController* const NewAIController);

};
