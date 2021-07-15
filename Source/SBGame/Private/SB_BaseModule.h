#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SB_BaseModule.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleSelectionUpdatedDelegate, bool, bNewIsSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleStateUpdatedDelegate, ESB_ModuleState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleDurabilityUpdatedDelegate, float, NewDurability);

class ASB_DataManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_BaseModule : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	USB_BaseModule();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

	//
	
	UFUNCTION()
	void ApplyDamage(const float Damage, const FVector& HitLocation, AController* const InstigatorController);

	//

	UFUNCTION() FName GetSlotName() const { return SlotName; }
	UFUNCTION() FName GetModuleName() const { return ModuleName; }
	FORCEINLINE UFUNCTION() const FSB_BaseModuleData* const GetBaseModuleData() const { return BaseModuleData; }
	UFUNCTION() ESB_ModuleState GetState() const { return State; }

	//

	FModuleSelectionUpdatedDelegate OnSelectionUpdated;
	FModuleStateUpdatedDelegate OnStateUpdated;
	FModuleDurabilityUpdatedDelegate OnDurabilityUpdated;

protected:

	const ASB_DataManager* DataManager;
	const FSB_BaseModuleData* BaseModuleData;
	TWeakObjectPtr<ASB_Ship> OwningShip;
	FTimerHandle RepairTimer;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ModuleName;

	//

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
