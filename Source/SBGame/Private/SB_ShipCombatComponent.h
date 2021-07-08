#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SB_ShipCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDurabilityUpdated, float, NewDurability);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class USB_ShipCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USB_ShipCombatComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	FDurabilityUpdated DurabilityUpdatedEvent;

private:

	const class ASB_DataManager* DataManager;

	//

	UPROPERTY()
	class ASB_Ship* OwnerShip;

	UPROPERTY(ReplicatedUsing = OnRep_Durability)
	float Durability;

	//

	UFUNCTION()
	void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnRep_Durability();
};
