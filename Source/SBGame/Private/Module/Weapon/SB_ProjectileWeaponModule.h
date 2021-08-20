#pragma once

#include "CoreMinimal.h"
#include "Module/Weapon/SB_WeaponModule.h"
#include "SB_ProjectileWeaponModule.generated.h"

UCLASS()
class USB_ProjectileWeaponModule : public USB_WeaponModule
{
	GENERATED_BODY()

public:

	USB_ProjectileWeaponModule();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UFUNCTION()
	void UpdateFire();
	
	UFUNCTION()
	void FireOnce();

	//

	UPROPERTY()
	bool bIsFiring;
	
	UPROPERTY()
	float LastFireTime;
	
};
