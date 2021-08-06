#pragma once

#include "Module/SB_BaseModule.h"
//
#include "CoreMinimal.h"
#include "SB_ShieldModule.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldDurabilityUpdatedDelegate, float, NewDurability, float, MaxDurability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldCooldownUpdatedDelegate, float, RemainingTime, float, MaxTime);

UCLASS()
class USB_ShieldModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_ShieldModule();

	/*virtual void Init(
		const ASB_DataManager* const NewDataManager,
		const FSB_ModuleSlotData* const NewModuleSlotData,
		const FName& NewModuleRowName) override;*/

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION()
	void StartSetup();

	UFUNCTION()
	void StopSetup();
	
	UFUNCTION()
	void Deploy();

	UFUNCTION(Server, Reliable)
	void Deploy_Server(float NewDeployedRotationYaw);

	UFUNCTION(NetMulticast, Reliable)
	void Deploy_Multicast(float NewDeployedRotationYaw);

	UFUNCTION()
	void Undeploy();

	UFUNCTION(Server, Reliable)
	void Undeploy_Server();

	UFUNCTION(NetMulticast, Reliable)
	void Undeploy_Multicast();

	//

	UFUNCTION()
	void ApplyShieldDamage(float Damage, const FVector& HitLocation, AController* const InstigatorController);

	UFUNCTION()
	void RegenOnce();

	//

	FORCEINLINE UFUNCTION() uint8 GetIsSetupMode() const { return bIsSetupMode; }
	FORCEINLINE UFUNCTION() ESB_ShieldState GetShieldState() const { return ShieldState; }
	FORCEINLINE UFUNCTION() float GetShieldDurability() const { return ShieldDurability; }

	//

	FShieldDurabilityUpdatedDelegate OnShieldDurabilityUpdated;
	FShieldCooldownUpdatedDelegate OnShieldCooldownUpdated;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ShieldArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SetupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASB_Shield* ShieldActor;

	//

	FTimerHandle ShieldRegenTimer;

	UPROPERTY()
	uint8 bIsSetupMode : 1;

	UPROPERTY()
	ESB_ShieldState ShieldState;

	UPROPERTY()
	float LastUndeployTime;

	UPROPERTY()
	float DeployedRotationYaw;

	UPROPERTY(ReplicatedUsing=OnRep_ShieldDurability)
	float ShieldDurability;

	//

	UFUNCTION()
	void OnRep_ShieldDurability() const;
};
