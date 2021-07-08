#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SB_BaseModule.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleSelectionUpdated, bool, bNewIsSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleDurabilityUpdated, float, bNewDurability);

UCLASS()
class USB_BaseModule : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	USB_BaseModule();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

	//

	FModuleSelectionUpdated ModuleSelectionUpdatedEvent;
	FModuleDurabilityUpdated ModuleDurabilityUpdatedEvent;
	
	//

	UFUNCTION()
	virtual void Init(const class ASB_DataManager* const NewDataManager, const FName& NewParentSlotName, const FName& NewDataRowName);

	UFUNCTION()
	void ApplyDamage(float Damage);

	//

	FORCEINLINE UFUNCTION() const FName& GetParentSlotName() const { return ParentSlotName; }
	FORCEINLINE UFUNCTION() const FSB_BaseModuleData* const GetBaseModuleData() const { return BaseModuleData; }

protected:

	const class ASB_DataManager* DataManager;
	const struct FSB_BaseModuleData* BaseModuleData;

	//
	
	FName ParentSlotName;
	FTimerHandle RepairTimer;

	//

	UPROPERTY()
	class ASB_Ship* OwnerShip;

	UPROPERTY(ReplicatedUsing=OnRep_Durability)
	float Durability;

	//

	UFUNCTION()
	void RepairOnce();

	UFUNCTION()
	void OnModuleDestroyed();

	UFUNCTION()
	void OnRep_Durability();
};
