#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SB_PlayerController.generated.h"

class USB_GameInstance;
class ARZ_UIManager;
class ASB_Vehicle;
class ARZ_CameraActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewOwnedVehicleDelegate, class ASB_Vehicle* const, NewShip);

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
	
	FORCEINLINE UFUNCTION() ARZ_UIManager* const GetUIManager() const { return UIManager; }
	FORCEINLINE UFUNCTION() ASB_Vehicle* const GetOwnedVehicle() const { return OwnedVehicle; }
	FORCEINLINE UFUNCTION() ARZ_CameraActor* const GetCameraActor() const { return CameraActor; }

	//

	FNewOwnedVehicleDelegate OnNewOwnedVehicle;

protected:
	
	USB_GameInstance* GInstance;
	ARZ_UIManager* UIManager;

	//

	UPROPERTY() 
	ASB_Vehicle* OwnedVehicle;

	UPROPERTY()
	ARZ_CameraActor* CameraActor;

///// Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void SetupInputComponent() override;

private:

	UFUNCTION() void LookUpAxis(float AxisValue);
	UFUNCTION() void LookRightAxis(float AxisValue);
	UFUNCTION() void ZoomAxis(float AxisValue);
	UFUNCTION() void MoveForwardAxis(float AxisValue);
	UFUNCTION() void MoveRightAxis(float AxisValue);
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
	
};
