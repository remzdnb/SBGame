#pragma once

#include "CoreMinimal.h"
#include "Module/Weapon/SB_WeaponModule.h"
#include "SB_BeamWeaponModule.generated.h"

UCLASS()
class USB_BeamWeaponModule : public USB_WeaponModule
{
	GENERATED_BODY()

public:

	virtual void Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	virtual void SetWantsToFire(bool bNewWantsToFire) override;

private:

	UFUNCTION()
	void IncreaseBeamLength(float DeltaTime);

	UFUNCTION()
	void DecreaseBeamLength(float DeltaTime);

	UFUNCTION()
	void CheckBeamCollision();
	
	//
	
	UPROPERTY()
	class ASB_Beam* BeamActor;

	UPROPERTY()
	class UParticleSystemComponent* BeamParticle;

	UPROPERTY()
	float CurrentBeamLength;
};
