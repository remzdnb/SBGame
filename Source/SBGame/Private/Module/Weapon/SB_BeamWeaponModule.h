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
	void CheckBeamCollision();
	
	UFUNCTION()
	void UpdateBeam();

	UFUNCTION()
	void StopBeam();

	UFUNCTION()
	void ApplyBeamDamage();
	
	//

	AController* OwnerController; // ToDo : set in base module ...

	UPROPERTY()
	class UParticleSystemComponent* BeamParticle;

	UPROPERTY()
	FHitResult HitResult;
	
	UPROPERTY()
	FTimerHandle DamageTimer;
};
