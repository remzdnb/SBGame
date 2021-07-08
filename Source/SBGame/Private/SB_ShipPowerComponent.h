#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SB_ShipPowerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoredPowerUpdated, uint8, NewStoredPower);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class USB_ShipPowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USB_ShipPowerComponent();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

	//

	FStoredPowerUpdated StoredPowerUpdatedEvent;

	//

	UFUNCTION()
	void Init(const class ASB_DataManager* const NewDataManager);

	UFUNCTION()
	void AssignPower(class USB_BaseModule* const BaseModuleReference);

	UFUNCTION()
	void RecoverPower(class USB_BaseModule* const BaseModuleReference);

	//

	FORCEINLINE UFUNCTION() uint8 GetMaxPower() const { return MaxPower; }
	FORCEINLINE UFUNCTION() uint8 GetStoredPower() const { return StoredPower; }

private:

	UPROPERTY()
	const class ASB_DataManager* DataManager;

	UPROPERTY() 
	class ASB_Ship* OwnerShip;

	//

	UPROPERTY()
	uint8 MaxPower;

	UPROPERTY()
	uint8 StoredPower;
};
