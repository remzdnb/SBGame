///// SB_AIController.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Interfaces.h"
//
#include "CoreMinimal.h"
#include "AIController.h"
#include "SB_AIController.generated.h"

class USB_GameInstance;
class ASB_BattleGameMode;
class ASB_GameState;
class ASB_PlayerState;
class ASB_Ship;

UCLASS()
class ASB_AIController : public AAIController, public ISB_ControllerInterface
{
	GENERATED_BODY()

public:

	ASB_AIController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	// ControllerInterface
	
	virtual uint8 GetTeamID() override;
	virtual ASB_Ship* const SpawnAndPossessVehicle(const FTransform& SpawnTransform) override;
	
	//

	FORCEINLINE UFUNCTION() ASB_PlayerState* GetPlayerState() const { return PState; }
	FORCEINLINE UFUNCTION() ASB_Ship* GetOwnedShip() const { return OwnedShip.Get(); }

private:

	USB_GameInstance* GInstance;
	ASB_BattleGameMode* GMode;
	ASB_GameState* GState;
	ASB_PlayerState* PState;

	//

	TWeakObjectPtr<ASB_Ship> OwnedShip;
	TWeakObjectPtr<ASB_Ship> TargetShip;
	TWeakObjectPtr<AActor> CollisionActor_Right;
	TWeakObjectPtr<AActor> CollisionActor_Left;

	//

	FTimerHandle DetectionUpdateTimer;

	//

	UFUNCTION()
	void UpdateDetection();

	UFUNCTION()
	void UpdateCollisionActor(bool bIsRightActor);

	UFUNCTION()
	void UpdateMovement();

	UFUNCTION()
	void UpdateTargetShip();
	
};
