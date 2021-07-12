///// SB_ShipCameraManager.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SB_ShipCameraManager.generated.h"

#define MINARMLENGTH 2000.0f
#define MAXARMLENGTH 60000.0f
#define DEFAULTARMLENGTH 25000.0f
#define ARMLENGTHSTEP 5000.0f
#define ARMLENGTHLERPSPEED 10.0f
#define MINPITCH -89.0f
#define MAXPITCH 89.0f

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShipCameraModeUpdated, bool, bIsSniperMode);

UCLASS()
class USB_ShipCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	USB_ShipCameraManager();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	void AddPitchInput(float AxisValue);
	void AddYawInput(float AxisValue);
	void Zoom(bool bZoomIn);
	void AddSniperCamera(class UCameraComponent* const NewActiveSniperCameraID);
	void SetActiveSniperCamera(uint8 WeaponCameraID);
	void ToggleSniperMode();

	//

	class USpringArmComponent* const GetCameraArmCT() { return MainCameraArm; } // meh
	bool GetIsSniperMode() const { return bIsSniperMode; }

	//

	//FShipCameraModeUpdated ShipCameraModeUpdatedEvent;

private:

	class USpringArmComponent* MainCameraArm;
	class UCameraComponent* MainCamera;
	TArray<UCameraComponent*> SniperCameras;

	//

	bool bIsSniperMode;
	uint8 ActiveSniperCameraID;
	float TargetArmLength;
	FRotator TargetArmRotation;
};
