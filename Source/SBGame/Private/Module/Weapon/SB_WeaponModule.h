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
	virtual void BeginPlay() override;
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

	UFUNCTION() // Update available targets, called by owning vehicle. // Server only.
	void SetTargets(TArray<ASB_Vehicle*> NewTargets);

protected:

	UPROPERTY()
	bool bIsAIEnabled;
	
	UPROPERTY() // ToDo : Convert to TWeakObjectPtr
	TArray<ASB_Vehicle*> TargetVehicles;
	
	TWeakObjectPtr<USB_TargetPoint> TargetPoint;

	UPROPERTY()
	FHitResult WeaponTraceResult;

private:
	
	UFUNCTION() // Search a line of view to a target point, from the vehicle targets list. // Server only.
	void UpdateTargetComponent(); // ToDo : TargetPoint or Component ?

	UFUNCTION() // Get hit result facing weapon muzzle, used by subclasses own logic. // Server only.
	void UpdateWeaponTrace();

	//

	FTimerHandle TargetUpdateTimer;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Rotation

protected:

	UPROPERTY()
	FRotator DefaultRotation;
	
	UPROPERTY()
	FRotator CurrentRotation;

private:

	UFUNCTION()
	void UpdateRotation(float DeltaTime);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Fire

public:
	
	UFUNCTION()
	virtual void SetWantsToFire(bool bNewWantsToFire);

protected:
	
	UPROPERTY()
	bool bWantsToFire;
};
