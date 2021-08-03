///// SB_ShipCameraManager.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SB_ShipCameraManager.generated.h"

#define MINARMLENGTH 10000.0f
#define MAXARMLENGTH 60000.0f
#define DEFAULTARMLENGTH 25000.0f
#define ARMLENGTHSTEP 5000.0f
#define ARMLENGTHLERPSPEED 10.0f
#define MINPITCH -89.0f
#define MAXPITCH 89.0f

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShipCameraModeUpdated, bool, bIsSniperMode);

class ASB_Ship;

UCLASS()
class USB_ShipCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	USB_ShipCameraManager();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION() void SetArmRotation(const FRotator& NewRotation, bool bShouldLerp = false);
	UFUNCTION() void AddPitchInput(float AxisValue);
	UFUNCTION() void AddYawInput(float AxisValue);

	UFUNCTION() void Zoom(bool bZoomIn);
	UFUNCTION() void SetArmRelativeLocation(const FVector& RelativeLocation);
	UFUNCTION() void AddSniperCamera(class UCameraComponent* const NewActiveSniperCameraID);
	UFUNCTION() void SetActiveSniperCamera(uint8 WeaponCameraID);
	UFUNCTION() void ToggleSniperMode();

	//

	FORCEINLINE void SetMinTargetArmLength() { TargetArmLength = 20000.0f; }
	FORCEINLINE void SetMaxTargetArmLength() { TargetArmLength = MAXARMLENGTH; }
	FORCEINLINE void SetTargetArmLength(float NewArmLength) { TargetArmLength = NewArmLength; }
	
	//

	class USpringArmComponent* const GetCameraArmCT() { return MainCameraArm; } // meh
	bool GetIsSniperMode() const { return bIsSniperMode; }

	//

	bool bLerpRotation;
	
	//FShipCameraModeUpdated ShipCameraModeUpdatedEvent;

private:

	TWeakObjectPtr<ASB_Ship> OwningShip;

	UPROPERTY() USpringArmComponent* MainCameraArm;
	UPROPERTY() UCameraComponent* MainCamera;
	UPROPERTY() TArray<UCameraComponent*> SniperCameras;

	//

	bool bIsSniperMode;
	uint8 ActiveSniperCameraID;
	float TargetArmLength;
	FVector TargetArmLocation;
	FRotator TargetArmRotation;
};
