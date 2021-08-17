#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_CameraActor.generated.h"

#define MINARMLENGTH 10000.0f
#define MAXARMLENGTH 60000.0f
#define DEFAULTARMLENGTH 25000.0f
#define ARMLENGTHSTEP 5000.0f
#define MINPITCH -89.0f
#define MAXPITCH 89.0f
#define LERPSPEED 5.0f

UCLASS()
class RZ_UTILITYPLUGIN_API ARZ_CameraActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_CameraActor();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//
	
	UFUNCTION() void AddPitchInput(float AxisValue);
	UFUNCTION() void AddYawInput(float AxisValue);
	UFUNCTION() void AddZoomInput(bool bZoomIn);

	UFUNCTION() void SetNewLocation(const FVector& NewLocation, bool bWithLerp);
	UFUNCTION() void SetNewRotation(const FRotator& NewRotation, bool bWithLerp);
	UFUNCTION() void SetNewArmLength(float NewArmLength, bool bWithLerp);

	//
	
	UPROPERTY() FVector TargetLocation;
	UPROPERTY() FRotator TargetArmRotation;
	UPROPERTY() float TargetArmLength;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	
};
