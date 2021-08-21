#pragma once

#include "SB_Interfaces.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Shield.generated.h"

UCLASS()
class ASB_Shield : public AActor, public ISB_CombatInterface
{
	GENERATED_BODY()
	
public:	

	ASB_Shield();

	virtual void BeginPlay() override;
	virtual void ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController) override;

	//

	UFUNCTION(BlueprintNativeEvent)
	void OnDeployedBPN();

	UFUNCTION(BlueprintNativeEvent)
	void OnDestroyedBPN();

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShieldMesh;

	//
	
	TWeakObjectPtr<class ASB_Vehicle> OwningShip;
	TWeakObjectPtr<class USB_ShieldModule> OwningShieldModule;

};
