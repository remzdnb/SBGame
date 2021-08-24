#pragma once

#include "CoreMinimal.h"
#include "Module/Weapon/SB_WeaponModule.h"
#include "SB_TraceWeaponModule.generated.h"

UCLASS()
class USB_TraceWeaponModule : public USB_WeaponModule
{
	GENERATED_BODY()

public:

	USB_TraceWeaponModule();

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
