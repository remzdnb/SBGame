#pragma once

#include "SB_Types.h"
#include "SB_Interfaces.h"
//
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SB_BaseModule.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectionUpdatedDelegate, bool, bNewIsSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleStateUpdatedDelegate, ESB_ModuleState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleDurabilityUpdatedDelegate, float, NewDurability);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_BaseModule : public USkeletalMeshComponent, public ISB_CombatInterface
{
	GENERATED_BODY()

public:

	USB_BaseModule();
	
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController) override;

	//

	UFUNCTION()
	void OnHoverStart();

	UFUNCTION()
	void OnHoverStop();

	UFUNCTION()
	void OnSelect();

	UFUNCTION()
	void OnUnselect();
	
	//

	FORCEINLINE UFUNCTION() const FSB_BaseModuleData* const GetBaseModuleData() const { return BaseModuleData; }
	FORCEINLINE UFUNCTION() ESB_ModuleState GetState() const { return State; }

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESB_SlotType SlotType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ModuleName;

	UPROPERTY()
	uint8 ModuleID;

	//

	FSelectionUpdatedDelegate OnSelectionUpdated;
	FModuleStateUpdatedDelegate OnStateUpdated;
	FModuleDurabilityUpdatedDelegate OnDurabilityUpdated;

protected:

	const class ASB_DataManager* DataManager;
	const FSB_BaseModuleData* BaseModuleData;
	
	TWeakObjectPtr<ASB_Ship> OwningShip;

	FTimerHandle RepairTimer;

	//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	bool bIsHovered;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	bool bIsSelected;

	UPROPERTY(ReplicatedUsing=OnRep_State)
	ESB_ModuleState State;
	
	UPROPERTY(ReplicatedUsing=OnRep_Durability)
	float Durability;

	//

	UFUNCTION()
	void RepairOnce();

	UFUNCTION()
	virtual void UpdateState(const ESB_ModuleState NewState);

	//

	UFUNCTION() void OnRep_State();
	UFUNCTION() void OnRep_Durability();
};
