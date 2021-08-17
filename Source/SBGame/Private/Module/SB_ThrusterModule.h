#pragma once

#include "CoreMinimal.h"
#include "Module/SB_BaseModule.h"
#include "SB_ThrusterModule.generated.h"

UCLASS()
class USB_ThrusterModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_ThrusterModule();

	virtual void Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName);
	
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float NewDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void UpdateState(const ESB_ModuleState NewState) override;

private:

	UFUNCTION()
	void UpdateExhaustParticle();
	
	UFUNCTION()
	void SetExhaustParticlesVisibility(bool bNewIsVisible);

	UFUNCTION()
	void Debug(float DeltaTime);

	//
	
	const FSB_ThrusterModuleData* ThrusterModuleData;
	const class USB_ShipMovementComponent* ShipMovement;

	//

	UPROPERTY()
	TArray<UParticleSystemComponent*> ExhaustParticles;
	
};
