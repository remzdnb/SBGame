///// SB_ShipMovementComponent.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SB_ShipMovementComponent.generated.h"

class ASB_DataManager;
class USB_ThrusterModule;

UCLASS()
class USB_ShipMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	USB_ShipMovementComponent();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION(Server, Reliable) //unreliable ?
	void MoveForward_Server(float AxisValue);

	UFUNCTION()
	void TurnRight(float AxisValue);

	UFUNCTION(Server, Reliable)
	void TurnRight_Server(float AxisValue);

	//

	FORCEINLINE UFUNCTION() float GetForwardAxisValue() const { return ForwardAxisValue; }
	FORCEINLINE UFUNCTION() float GetRightAxisValue() const { return RightAxisValue; }
	FORCEINLINE UFUNCTION() float GetTargetRotationYaw() const { return TargetRotationYaw; }

private:

	const ASB_DataManager* DataManager;
	TWeakObjectPtr<ASB_Ship> OwningShip;
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	//
	
	USB_ThrusterModule* ThrusterModule_Back;
	USB_ThrusterModule* ThrusterModule_Front;
	USB_ThrusterModule* ThrusterModule_Left;
	USB_ThrusterModule* ThrusterModule_Right;
	
	//

	UPROPERTY(Replicated)
	float ForwardAxisValue;

	UPROPERTY(Replicated)
	float RightAxisValue;

	UPROPERTY(Replicated)
	float TargetRotationYaw;

	float DeltaLol;
	
	//

	UFUNCTION()
	void TickMovement(float DeltaTime);

	UFUNCTION()
	void Debug(float DeltaTime);
};
