///// SB_WeaponModule.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "SB_BaseModule.h"
//
#include "CoreMinimal.h"
#include "SB_WeaponModule.generated.h"

class ASB_Ship;
class ASB_DataManager;

UCLASS()
class USB_WeaponModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_WeaponModule();

	virtual void Init(const ASB_DataManager* const NewDataManager, const FName& NewParentSlotName, const FName& NewDataRowName) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION()
	void SetIsSelected(bool bToggleSelection, bool bNewIsSelected);

	UFUNCTION()
	void SetWantsToFire(bool bNewWantsToFire);

	UFUNCTION()
	void SetTargetShip(class ASB_Ship* const NewTargetShip);

	UFUNCTION()
	void ToggleSniperView(bool bNewIsSniperView);

	//

	FORCEINLINE UFUNCTION() bool GetIsSelected() const { return bIsSelected; }

private:

	const FSB_WeaponModuleData* WeaponModuleData;
	class USB_WeaponModuleAnimInstance* WeaponAnimInstance;

	//

	UPROPERTY()
	ASB_Ship* TargetShip;

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
