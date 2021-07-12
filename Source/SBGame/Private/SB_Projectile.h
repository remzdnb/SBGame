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

	FORCEINLINE UFUNCTION() class UProjectileMovementComponent* const GetProjectileMovementCT() const { return ProjectileMovementCT; }

private:

	// Components

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphereCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementCT;

	// Data

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataRowName;

	const FSB_ProjectileData* ProjectileData;

	//

	UPROPERTY()
	class ASB_Ship* OwnerShip;

	//

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void SpawnImpactFX_Multicast(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal);

	UFUNCTION()
	void Debug(AActor* HitActor);
};
