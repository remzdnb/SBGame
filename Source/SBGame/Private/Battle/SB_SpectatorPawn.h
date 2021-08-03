#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "SB_SpectatorPawn.generated.h"

UCLASS()
class ASB_SpectatorPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	ASB_SpectatorPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MoveForward(float AxisValue) override;
	virtual void MoveRight(float AxisValue) override;

	UFUNCTION() void Zoom(float AxisValue);
	UFUNCTION() void ToggleQuickMovement(bool bIsEnabled);

private:

	class UFloatingPawnMovement* FloatingPawnMovement;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArmCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraCT;

	//

	UPROPERTY() float DefaultMovementSpeed;
	UPROPERTY() float QuickMovementSpeed;
	UPROPERTY() float TargetArmLength;
	UPROPERTY() float ZoomStep;
	UPROPERTY() float ZoomSpeed;
};
