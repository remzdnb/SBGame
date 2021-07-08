#pragma once

#include "CoreMinimal.h"
#include "SB_BaseModule.h"
#include "SB_ThrusterModule.generated.h"

UCLASS()
class USB_ThrusterModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_ThrusterModule();

	virtual void Init(const class ASB_DataManager* const NewDataManager, const FName& NewParentSlotName, const FName& NewDataRowName) override;

	virtual void TickComponent(float NewDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	const FSB_ThrusterModuleData* ThrusterModuleData;
	const class USB_ShipMovementComponent* ShipMovementCT;

	//

	UPROPERTY()
	TArray<UParticleSystemComponent*> ExhaustParticles;

	//

	UFUNCTION()
	void SetExhaustParticlesVisibility(bool bNewIsVisible);

	UFUNCTION()
	void Debug(float DeltaTime);

};
