///// SB_ShipMovementComponent.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SB_ShipMovementComponent.generated.h"

UCLASS()
class USB_ShipMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	USB_ShipMovementComponent();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float NewDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION(Server, Reliable)
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

	const class ASB_DataManager* DataManager;

	//

	UPROPERTY()
	class ASB_Ship* OwnerShip;

	//

	UPROPERTY(Replicated)
	float ForwardAxisValue;

	UPROPERTY(Replicated)
	float RightAxisValue;

	UPROPERTY(Replicated)
	float TargetRotationYaw;

	//

	UFUNCTION()
	void TickMovement(float DeltaTime);

	UFUNCTION()
	void Debug(float DeltaTime);
};
