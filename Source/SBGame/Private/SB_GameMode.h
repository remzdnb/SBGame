#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SB_GameMode.generated.h"

class ASB_PlayerController;
class ASB_AIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameInitialized, class ASB_Ship* const, PlayerShip, class ASB_Ship* const, EnemyShip);

UCLASS()
class ASB_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASB_GameMode();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayerController) override;

	//

	UFUNCTION()
	void QueryRespawn(AController* const NewController);

	//

	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_PlayerController>> const GetPlayerControllers() { return PlayerControllers; }
	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_AIController>> const GetAIControllers() { return AIControllers; }

private:

	const class ASB_DataManager* DataManager;
	TArray<class ASB_ShipStart*> ShipStarts;
	FTimerHandle SpawnTimer;

	// "Registered" controllers are those ready to spawn their ships.

	TArray<TWeakObjectPtr<ASB_PlayerController>> PlayerControllers;
	TArray<TWeakObjectPtr<ASB_PlayerController>> RegisteredPlayerControllers;
	TArray<TWeakObjectPtr<ASB_AIController>> AIControllers;
	TArray<TWeakObjectPtr<ASB_AIController>> RegisteredAIControllers;

	//

	UFUNCTION()
	class ASB_ShipStart* GetAvailableShipStart(uint8 TeamID);

	UFUNCTION(Exec)
	void SpawnAIController(uint8 TeamID);

	UFUNCTION(Exec)
	void RemoveAIController(uint8 TeamID);

};
