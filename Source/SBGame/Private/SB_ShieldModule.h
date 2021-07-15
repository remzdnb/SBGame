#pragma once

#include "SB_BaseModule.h"
//
#include "CoreMinimal.h"
#include "SB_ShieldModule.generated.h"

UCLASS()
class USB_ShieldModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_ShieldModule();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION()
	void StartSetup();

	UFUNCTION()
	void StopSetup();

	UFUNCTION()
	void StartDeploy();
	
	UFUNCTION()
	void Deploy(const float NewDeployedRotationYaw);

	UFUNCTION(Server, Reliable)
	void Deploy_Server(const float NewDeployedRotationYaw);

	UFUNCTION(NetMulticast, Reliable)
	void Deploy_Multicast(const float NewDeployedRotationYaw);

	UFUNCTION()
	void Undeploy();

	UFUNCTION(Server, Reliable)
	void Undeploy_Server();

	UFUNCTION(NetMulticast, Reliable)
	void Undeploy_Multicast();

	//

	FORCEINLINE UFUNCTION() uint8 GetIsSetupMode() const { return bIsSetupMode; }
	FORCEINLINE UFUNCTION() uint8 GetIsDeployed() const { return bIsDeployed; }
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ShieldArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SetupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASB_Shield* ShieldActor;

	//

	UPROPERTY()
	uint8 bIsSetupMode : 1;

	UPROPERTY()
	uint8 bIsDeployed : 1;

	UPROPERTY()
	float DeployedRotationYaw;

	UPROPERTY(ReplicatedUsing=OnRep_ShieldDurability)
	float ShieldDurability;

	//

	UFUNCTION()
	void OnRep_ShieldDurability() const;
};
