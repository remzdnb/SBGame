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
	void Deploy();

	//

	FORCEINLINE UFUNCTION() bool GetIsSetupMode() const { return bIsSetupMode; }
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ShieldArmCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASB_Shield* ShieldActor;

	//

	UPROPERTY()
	bool bIsSetupMode;

	UPROPERTY()
	bool bIsDeployed;

	UPROPERTY()
	FRotator DeployedRotation;


};
