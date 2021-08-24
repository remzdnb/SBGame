#pragma once

#include "SB_GameMode.h"
//
#include "CoreMinimal.h"
#include "SB_BattleGameMode.generated.h"

class ASB_ShipStart;
class ASB_BattlePlayerController;
class ASB_AIController;
class ASB_Vehicle;

UCLASS()
class ASB_BattleGameMode : public ASB_GameMode
{
	GENERATED_BODY()

public:

	ASB_BattleGameMode();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayerController) override;
	
	//

	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_BattlePlayerController>> const GetPlayerControllers() { return PlayerControllers; }
	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_AIController>> const GetAIControllers() { return AIControllers; }
	FORCEINLINE UFUNCTION() TArray<TWeakObjectPtr<ASB_Vehicle>> const GetSpawnedShips() { return SpawnedVehicles; }

protected:

	TArray<ASB_ShipStart*> ShipStarts;
	TArray<TWeakObjectPtr<ASB_BattlePlayerController>> PlayerControllers;
	TArray<TWeakObjectPtr<ASB_AIController>> AIControllers;
	TArray<TWeakObjectPtr<ASB_Vehicle>> SpawnedVehicles;
	
	//

	UFUNCTION()
	ASB_ShipStart* GetAvailableShipStart(uint8 TeamID);

	UFUNCTION()
	void RegisterVehicle(AActor* const VehicleToRegister);

	UFUNCTION()
	virtual void UnregisterVehicle(ASB_Vehicle* const VehicleToUnregister, AController* OwningController);

	UFUNCTION(Exec)
	void SpawnAIController(uint8 TeamID);

	UFUNCTION(Exec)
	void RemoveAIController(uint8 TeamID);

	UFUNCTION()
	virtual void OnAIControllerSpawned(ASB_AIController* const NewAIController);

};
