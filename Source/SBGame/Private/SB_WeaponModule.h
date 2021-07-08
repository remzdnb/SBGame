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

	//virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void Init(const class ASB_DataManager* const NewDataManager, const FName& NewDataRowName) override;

	//

	UFUNCTION()
	void ToggleSelection();

	UFUNCTION()
	void SetWantsToFire(bool bNewWantsToFire);

private:

	UPROPERTY() FSB_WeaponModuleData WeaponModuleData;
	//USB_WeaponModuleAnimInstance* AInstance;
	UPROPERTY() float LastFireTime;
	UPROPERTY() bool bIsSelected;
	UPROPERTY() bool bWantsToFire;
	UPROPERTY() bool bIsFiring;

	//

	UFUNCTION()
	void FireTick();

	UFUNCTION()
	void FireOnce();

	UFUNCTION()
	void Debug(float DeltaTime);
};
