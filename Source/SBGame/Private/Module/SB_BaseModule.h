#pragma once

#include "SB_Types.h"
#include "SB_Interfaces.h"
//
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SB_BaseModule.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FModuleDamaged,
	class USB_BaseModule* const, ModuleRef,
	float, Damage,
	const FVector&, HitLocation,
	class AController* const, InstigatorController
);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleStateUpdatedDelegate, ESB_ModuleState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleDurabilityUpdatedDelegate, float, NewDurability);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_BaseModule : public USkeletalMeshComponent, public ISB_CombatInterface
{
	GENERATED_BODY()

public:

	USB_BaseModule();

	virtual void Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName); // Rename to setup
	virtual void InitializeComponent() override;

	//

	UFUNCTION()
	void OnHoverStart();

	UFUNCTION()
	void OnHoverStop();

	UFUNCTION()
	void ToggleHighlight(bool bNewIsEnabled = true);

protected:
	
	const class USB_GameInstance* GInstance;
	const class ASB_GameState* GState;
	
	TWeakObjectPtr<ASB_Vehicle> OwningShip;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Data

public:
	
	FORCEINLINE UFUNCTION() const FSB_ModuleSlotData& GetModuleSlotData() const { return ModuleSlotData; }
	FORCEINLINE UFUNCTION() const FSB_BaseModuleData* const GetBaseModuleData() const { return BaseModuleData; }
	FORCEINLINE UFUNCTION() const FName& GetModuleRowName() const { return ModuleRowName; }

protected:

	FSB_ModuleSlotData ModuleSlotData;
	FName ModuleRowName;
	const FSB_BaseModuleData* BaseModuleData;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// State

public:

	UFUNCTION()
	virtual void UpdateState(const ESB_ModuleState NewState);

	FORCEINLINE UFUNCTION() ESB_ModuleState GetState() const { return State; }

	//

	FModuleStateUpdatedDelegate OnStateUpdated;

private:
	
	UFUNCTION()
	void OnRep_State();

	//
	
	UPROPERTY(ReplicatedUsing=OnRep_State)
	ESB_ModuleState State;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Combat

public:
	
	virtual void ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController) override;

	//

	FModuleDamaged OnModuleDamaged;
	FModuleDurabilityUpdatedDelegate OnDurabilityUpdated;

private:

	UFUNCTION()
	void RepairOnce();

	UFUNCTION()
	void OnRep_Durability();

	//

	FTimerHandle RepairTimer;

	UPROPERTY(ReplicatedUsing=OnRep_Durability)
	float Durability;
	
};
