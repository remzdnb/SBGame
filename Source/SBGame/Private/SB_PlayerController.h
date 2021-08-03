///// SB_PlayerController.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "SB_Types.h"
#include "GameFramework/PlayerController.h"
#include "SB_PlayerController.generated.h"

class ASB_DataManager;
class ASB_GameMode;
class ASB_GameState;
class ASB_PlayerState;
class ARZ_UIManager;
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
	void OnDamageDealt(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType);

	UFUNCTION(Client, Reliable)
	void OnDamageDealt_Client(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType);

	//

	FORCEINLINE UFUNCTION() ASB_PlayerState* const GetPlayerState() const { return PState; }
	FORCEINLINE UFUNCTION() ARZ_UIManager* const GetUIManager() const { return UIManager; }
	FORCEINLINE UFUNCTION() ASB_Ship* const GetOwnedShip() const { return OwnedShip; }

	//

	FNewOwnedShipDelegate OnNewOwnedShip;

protected:

	ASB_DataManager* DataManager;
	ASB_GameMode* GMode;
	ASB_GameState* GState;
	ASB_PlayerState* PState;
	ARZ_UIManager* UIManager;

	//

	UPROPERTY()
	class ADefaultPawn* OwnedSpectatorPawn;

	UPROPERTY() 
	class ASB_Ship* OwnedShip;

	//



	UFUNCTION()
	void OnOwnedShipDestroyed(const class APlayerState* const InstigatorPS);

	UFUNCTION(Server, Reliable)
	void Respawn_Server();

///// Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void SetupInputComponent() override;

	FORCEINLINE UFUNCTION() class USB_CursorWidget* GetCursorWidget() const { return CursorWidget; }

private:

	UFUNCTION() void LookUpAxis(float AxisValue);
	UFUNCTION() void LookRightAxis(float AxisValue);
	UFUNCTION() void MoveForwardAxis(float AxisValue);
	UFUNCTION() void MoveRightAxis(float AxisValue);
	UFUNCTION() void MouseWheelAxis(float AxisValue);
	UFUNCTION() void RightMouseButtonPressed();
	UFUNCTION() void RightMouseButtonReleased();
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

	//

	UPROPERTY() class USB_CursorWidget* CursorWidget;
};
