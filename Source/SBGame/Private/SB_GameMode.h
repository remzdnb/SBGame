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

	const class ASB_DataManager* DataManager;
	TArray<class ASB_ShipStart*> ShipStarts;
	FTimerHandle SpawnTimer;

	//

	UPROPERTY()
	TArray<class ASB_AIController*> AIControllers;

	UPROPERTY()
	TArray<class ASB_AIController*> RegisteredAIControllers;

	UPROPERTY()
	TArray<class ASB_PlayerController*> RegisteredPlayerControllers;

	//

	UFUNCTION()
	class ASB_ShipStart* GetAvailableShipStart(uint8 TeamID);

	UFUNCTION(Exec)
	void SpawnAIController(uint8 TeamID);

	UFUNCTION(Exec)
	void RemoveAIController(uint8 TeamID);
};
