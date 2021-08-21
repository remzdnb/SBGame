#pragma once

#include "SB_Types.h"
#include "Module/SB_BaseModule.h"
//
#include "CoreMinimal.h"
#include "SB_WeaponModule.generated.h"

#define TARGETUPDATERATE 0.5f

class ASB_Vehicle;
class USB_TargetPoint;
class USB_WeaponModuleAnimInstance;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_WeaponModule : public USB_BaseModule
{
	GENERATED_BODY()

public:

	USB_WeaponModule();

	virtual void Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	FORCEINLINE UFUNCTION() const FSB_WeaponModuleData* const GetWeaponModuleData() const { return WeaponModuleData; }

protected:

	const FSB_WeaponModuleData* WeaponModuleData;

private:
	
	USB_WeaponModuleAnimInstance* WeaponAnimInstance;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Targeting

public:

	UFUNCTION()
	void SetTargetShip(ASB_Vehicle* const NewTargetShip);

protected:
	
	TWeakObjectPtr<ASB_Vehicle> TargetShip;
	TWeakObjectPtr<USB_TargetPoint> TargetPoint;

private:
	
	UFUNCTION()
	void UpdateTargetComponent();

	//

	FTimerHandle TargetUpdateTimer;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Rotation

protected:

	UPROPERTY()
	FRotator LerpedRotation;

private:

	UFUNCTION()
	void UpdateRotation();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Fire

public:
	
	UFUNCTION()
	virtual void SetWantsToFire(bool bNewWantsToFire);

protected:
	
	UPROPERTY()
	bool bWantsToFire;
};
