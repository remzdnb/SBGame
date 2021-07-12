///// SB_PlayerController.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SB_PlayerController.generated.h"

class ASB_DataManager;
class ASB_GameMode;
class ASB_GameState;
class ASB_PlayerState;
class ASB_UIManager;
class ASB_Ship;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewOwnedShipDelegate, ASB_Ship* const, NewShip);

UCLASS()
class ASB_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;

	//

	UFUNCTION()
	void SpawnAndPossessShip(const FTransform& SpawnTransform);

	UFUNCTION()
	void OnDamageDealt(const FVector& HitLocation, float Damage);

	UFUNCTION(Client, Reliable)
	void OnDamageDealt_Client(const FVector& HitLocation, float Damage);

	//

	FORCEINLINE UFUNCTION() ASB_PlayerState* const GetPlayerState() const { return PState; }
	FORCEINLINE UFUNCTION() ASB_UIManager* const GetUIManager() const { return UIManager; }
	FORCEINLINE UFUNCTION() ASB_Ship* const GetOwnedShip() const { return OwnedShip; }

	//

	FNewOwnedShipDelegate OnNewOwnedShip;

private:

	ASB_DataManager* DataManager;
	ASB_GameMode* GMode;
	ASB_GameState* GState;
	ASB_PlayerState* PState;
	ASB_UIManager* UIManager;

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
	UFUNCTION() void TabKeyPressed();
	UFUNCTION() void ShiftKeyPressed();
	UFUNCTION() void ShiftKeyReleased();
	UFUNCTION() void SpaceBarKeyPressed();
	UFUNCTION() void SelectWeapon1KeyPressed();
	UFUNCTION() void SelectWeapon2KeyPressed();
	UFUNCTION() void SelectWeapon3KeyPressed();
	UFUNCTION() void SelectWeapon4KeyPressed();
	UFUNCTION() void SelectWeapon5KeyPressed();
	UFUNCTION() void SelectAllWeaponsKeyPressed();
	UFUNCTION() void UnselectAllWeaponsKeyPressed();
};
