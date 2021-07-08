#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SB_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewOwnedShip, class ASB_Ship* const, NewShip);

UCLASS()
class ASB_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FNewOwnedShip NewOwnedShipEvent;

	//

	UFUNCTION()
	void SpawnAndPossessShip(const FTransform& SpawnTransform);

	//

	FORCEINLINE UFUNCTION() class ASB_Ship* const GetOwnedShip() const { return OwnedShip; }

private:

	class ASB_DataManager* DataManager;
	class ARZ_UIManager* UIManager;
	class ASB_GameMode* GMode;

	//

	UPROPERTY()
	class ADefaultPawn* OwnedSpectatorPawn;

	UPROPERTY() 
	class ASB_Ship* OwnedShip;

	//

	UFUNCTION()
	void UpdateViewTarget();

	UFUNCTION()
	void OnOwnedShipDestroyed(const class APlayerState* const InstigatorPS);

	UFUNCTION(Server, Reliable)
	void Respawn_Server();

///// Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void SetupInputComponent() override;

private:

	UFUNCTION() void LookUpAxis(float AxisValue);
	UFUNCTION() void LookRightAxis(float AxisValue);
	UFUNCTION() void MoveForwardAxis(float AxisValue);
	UFUNCTION() void MoveRightAxis(float AxisValue);
	UFUNCTION() void MouseWheelAxis(float AxisValue);
	UFUNCTION() void LeftMouseButtonPressed();
	UFUNCTION() void LeftMouseButtonReleased();
	UFUNCTION() void RightMouseButtonPressed();
	UFUNCTION() void RightMouseButtonReleased();
	UFUNCTION() void MouseWheelUp();
	UFUNCTION() void MouseWheelDown();
	//UFUNCTION() void TabKeyPressed();
	UFUNCTION() void ShiftKeyPressed();
	UFUNCTION() void ShiftKeyReleased();
	UFUNCTION() void SpaceBarKeyPressed();
	UFUNCTION() void Quickslot1KeyPressed();
	UFUNCTION() void Quickslot2KeyPressed();
	UFUNCTION() void Quickslot3KeyPressed();
	UFUNCTION() void Quickslot4KeyPressed();
	UFUNCTION() void Quickslot5KeyPressed();
};
