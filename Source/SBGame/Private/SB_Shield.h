#pragma once

#include "SB_Interfaces.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Shield.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldDurabilityUpdatedDelegate, const float, NewDurability, const float, MaxDurability);

UCLASS()
class ASB_Shield : public AActor, public ISB_CombatInterface
{
	GENERATED_BODY()
	
public:	

	ASB_Shield();

	virtual void BeginPlay() override;

	virtual void ApplyDamage(const float Damage, const FVector& HitLocation, AController* const InstigatorController) override;

	//

	UFUNCTION(BlueprintNativeEvent)
	void OnDeployedBPN();

	UFUNCTION(BlueprintNativeEvent)
	void OnDestroyedBPN();

	//

	FShieldDurabilityUpdatedDelegate OnDurabilityUpdated;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShieldMesh;

	//

	const class ASB_DataManager* DataManager;

	//


};
