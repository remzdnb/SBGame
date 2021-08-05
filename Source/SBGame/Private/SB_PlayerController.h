#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SB_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewOwnedShipDelegate, class ASB_Ship* const, NewShip);

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

	FORCEINLINE UFUNCTION() class ASB_PlayerState* const GetPlayerState() const { return PState; }
	FORCEINLINE UFUNCTION() class ARZ_UIManager* const GetUIManager() const { return UIManager; }
	FORCEINLINE UFUNCTION() class ASB_Ship* const GetOwnedShip() const { return OwnedShip; }

	//

	FNewOwnedShipDelegate OnNewOwnedShip;

protected:

	class ASB_DataManager* DataManager;
	class USB_GameInstance* GInstance;
	class ASB_GameMode* GMode;
	class ASB_GameState* GState;
	class ASB_PlayerState* PState;
	class ARZ_UIManager* UIManager;

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
