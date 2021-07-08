#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_MeshRenderer.generated.h"

UCLASS()
class ARZ_MeshRenderer : public AActor
{
	GENERATED_BODY()

public:

	ARZ_MeshRenderer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void SetArmLength(float NewArmLength);
	UFUNCTION(BlueprintCallable) void SetCameraOrthoWidth(float NewOrthoWidth);
	UFUNCTION(BlueprintCallable) void SetActorPitch(float NewPitch);
	UFUNCTION(BlueprintCallable) void SetActorYaw(float NewYaw);
	UFUNCTION(BlueprintCallable) void SetActorRoll(float NewRoll);
	UFUNCTION(BlueprintCallable) void AddActorYOffset(float NewXOffset);
	UFUNCTION(BlueprintCallable) void AddActorZOffset(float NewZOffset);

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArmCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* CaptureCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowCT;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ASkeletalMeshActor* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AStaticMeshActor* StaticMesh;
};
