///// SB_FSGameMode.h - RemzDNB
///// Free spawn GameMode, for testing purposes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Interfaces.h"
#include "Battle/SB_BattleGameMode.h"
//
#include "CoreMinimal.h"
#include "SB_FSGameMode.generated.h"

UCLASS()
class ASB_FSGameMode : public ASB_BattleGameMode
{
	GENERATED_BODY()

public:

	ASB_FSGameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayerController) override;

	//

	UFUNCTION()
	void QueryFreeRespawnWithDelay(AController* const NewController, float Delay);
	
	UFUNCTION()
	void QueryFreeRespawn(AController* const NewController);

private:

	virtual void OnAIControllerSpawned(ASB_AIController* const NewAIController) override;
	virtual void UnregisterVehicle(ASB_Vehicle* const VehicleToUnregister, AController* OwningController) override;

	//
	
	UFUNCTION()
	void HandleFreeRespawn();

	//

	TArray<TWeakObjectPtr<AController>> ReadyToSpawnControllers;
};
