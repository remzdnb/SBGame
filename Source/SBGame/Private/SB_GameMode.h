#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SB_GameMode.generated.h"

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

	//

	UFUNCTION()
	void QueryRespawn(AController* const NewController);

private:

	class ASB_DataManager* DataManager;

	UPROPERTY() TArray<class ASB_ShipStart*> ShipStartArray;

	UPROPERTY() TArray<TWeakObjectPtr<class ASB_PlayerController>> RegisteredPlayerControllers;
	UPROPERTY() TArray<TWeakObjectPtr<class AAIController>> RegisteredAIControllers;

	FTimerHandle SpawnTimer;

	//

	UFUNCTION() // Returns nullptr if no one has been found.
	class ASB_ShipStart* GetAvailableShipStart(uint8 TeamID);

	/*UFUNCTION(Exec)
	void SpawnAIController(uint8 Team);

	UFUNCTION(Exec)
	void RemoveAIController(uint8 Team);*/
};
