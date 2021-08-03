///// SB_BaseWeaponModule.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "Module/SB_BaseModule.h"
//
#include "CoreMinimal.h"
#include "SB_BaseWeaponModule.generated.h"

class ASB_Ship;
class ASB_DataManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_BaseWeaponModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_BaseWeaponModule();

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION()
	void SetIsWeaponSelected(bool bToggleSelection, bool bNewIsSelected);

	UFUNCTION()
	void SetWantsToFire(bool bNewWantsToFire);

	UFUNCTION()
	void SetTargetShip(class ASB_Ship* const NewTargetShip);

	UFUNCTION()
	void ToggleSniperView(bool bNewIsSniperView);

	//

	FORCEINLINE UFUNCTION() bool GetIsSelected() const { return bIsSelected; }

private:

	const FSB_BaseWeaponModuleData* WeaponModuleData;
	class USB_WeaponModuleAnimInstance* WeaponAnimInstance;

	//

	UPROPERTY()
	class ASB_Ship* TargetShip;

	UPROPERTY()
	const class USceneComponent* TargetAutoLockCT;

	UParticleSystemComponent* MuzzleParticle;

	UPROPERTY() bool bIsSniperView;
	UPROPERTY() bool bWantsToFire;
	UPROPERTY() bool bIsFiring;
	UPROPERTY() float LastFireTime;

	//

	UFUNCTION()
	void SelectAutoLockCT();

	UFUNCTION()
	void FireTick();

	UFUNCTION()
	void FireOnce();

	UFUNCTION()
	void Debug(float DeltaTime);
};
