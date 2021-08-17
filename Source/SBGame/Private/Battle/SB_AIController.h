///// SB_AIController.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SB_GameInstance.h"
#include "SB_AIController.generated.h"

class ASB_GameMode;
class ASB_GameState;
class ASB_PlayerState;
class ASB_Ship;

UCLASS()
class ASB_AIController : public AAIController
{
	GENERATED_BODY()

public:

	ASB_AIController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	//

	UFUNCTION()
	void SpawnAndPossessShip(const FTransform& SpawnTransform);

	//

	FORCEINLINE UFUNCTION() ASB_PlayerState* GetPlayerState() const { return PState; }
	FORCEINLINE UFUNCTION() TWeakObjectPtr<ASB_Ship> GetOwnedShip() const { return OwnedShip; }

private:

	USB_GameInstance* GInstance;
	ASB_GameMode* GMode;
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
