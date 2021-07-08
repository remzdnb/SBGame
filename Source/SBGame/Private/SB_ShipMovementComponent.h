///// SB_ShipMovementComponent.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SB_ShipMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShipMovementUpdated, int32, ForwardInput, int32, RightInput);

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

	FShipMovementUpdated ShipMovementUpdatedEvent;

	//

	UFUNCTION()
	void AddForwardInput(int32 ForwardInputToAdd);

	UFUNCTION(Server, Reliable)
	void AddForwardInput_Server(int32 ForwardInputToAdd);

	UFUNCTION()
	void AddRightInput(int32 RightInputToAdd);

	UFUNCTION(Server, Reliable)
	void AddRightInput_Server(int32 RightInputToAdd);

	//

	FORCEINLINE UFUNCTION() float GetRotationYaw() const { return RotationYaw; }

private:

	UPROPERTY() class ASB_DataManager* DataManager;
	UPROPERTY() class ASB_Ship* OwnerShip;

	//

	UPROPERTY() int32 MaxForwardInput;
	UPROPERTY() int32 MinForwardInput;
	UPROPERTY() int32 MaxRightInput;

	UPROPERTY()
	int32 ForwardInput;

	UPROPERTY()
	int32 RightInput;

	UPROPERTY(Replicated)
	float RotationYaw;

	//

	UFUNCTION()
	void TickMovement(float DeltaTime);

	UFUNCTION()
	void Debug(float DeltaTime);
};
