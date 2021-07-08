///// SB_ShipCameraComponent.h - RemzDNB
///// Designed to work as a standalone Component of any Pawn/Character with a SpringArm + Camera setup.
///// Manages "local" CameraComponent only, mostly by offsetting it from its SpringArm.
///// For camera stuff outside the Pawn scope, like switching to another Camera, one must use the global CameraManager instead.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SB_ShipCameraComponent.generated.h"

#define DEFAULTARMLENGTH 15000.0f
#define ARMLENGTHLERPSPEED 10.0f
#define ARMLENGTHSTEP 1000.0f
#define MINPITCH -89.0f
#define MAXPITCH 0.0f

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class USB_ShipCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USB_ShipCameraComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	FORCEINLINE UFUNCTION() class USpringArmComponent* const GetCameraArmCT() { return CameraArmCT; }

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
public:

	UFUNCTION() void AddPitchInput(float AxisValue);
	UFUNCTION() void AddYawInput(float AxisValue);
	UFUNCTION() void ZoomIn();
	UFUNCTION() void ZoomOut();

private:

	class USpringArmComponent* CameraArmCT;
	class UCameraComponent* CameraCT;

	//

	UPROPERTY() float TargetArmLength;
	UPROPERTY() FRotator TargetArmRotation;
};
