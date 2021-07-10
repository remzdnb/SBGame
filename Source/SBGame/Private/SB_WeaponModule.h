#pragma once

#include "SB_Types.h"
#include "SB_BaseModule.h"
//
#include "CoreMinimal.h"
#include "SB_WeaponModule.generated.h"

UCLASS()
class USB_WeaponModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_WeaponModule();

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void Init(const class ASB_DataManager* const NewDataManager, const FName& NewParentSlotName, const FName& NewDataRowName) override;

	//

	UFUNCTION()
	void ToggleSelection(bool bNewIsSelected);

	UFUNCTION()
	void ToggleSniperView(bool bNewIsSniperView);

	UFUNCTION()
	void SetWantsToFire(bool bNewWantsToFire);

	UFUNCTION()
	void SetTargetShip(class ASB_Ship* const NewTargetShip);

private:

	const FSB_WeaponModuleData* WeaponModuleData;
	class USB_WeaponModuleAnimInstance* WeaponAnimInstance;

	//

	UPROPERTY()
	class ASB_Ship* TargetShip;

	UPROPERTY()
	const class USceneComponent* TargetAutoLockCT;

	UParticleSystemComponent* MuzzleParticle;

	UPROPERTY() bool bIsSelected;
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
