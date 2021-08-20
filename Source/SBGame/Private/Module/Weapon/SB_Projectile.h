#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Projectile.generated.h"

UCLASS()
class ASB_Projectile : public AActor
{
	GENERATED_BODY()

public:	

	ASB_Projectile();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	//

	FORCEINLINE UFUNCTION() class UProjectileMovementComponent* const GetProjectileShipMovement() const { return ProjectileMovement; }

private:

	// Components

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* VisualParticle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	// Data

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataRowName;

	const FSB_WeaponModuleData* WeaponModuleData;

	//

	class USB_GameInstance* GInstance;
	
	TWeakObjectPtr<AController> OwningController;
	TWeakObjectPtr<ASB_Ship> OwningShip;

	//

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void SpawnImpactFX_Multicast(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal);

	UFUNCTION()
	void Debug(AActor* HitActor);
};
