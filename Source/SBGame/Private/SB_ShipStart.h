///// SB_ShipStart.h - RemzDNB
///// Ship spawn location.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_ShipStart.generated.h"

UCLASS()
class ASB_ShipStart : public AActor
{
	GENERATED_BODY()
	
public:	

	ASB_ShipStart();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	//

	UFUNCTION(NetMulticast, Reliable)
	void UpdateAvailability_Multicast(bool bNewIsAvailable);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAvailability_BPI(bool bNewIsAvailable);

	FORCEINLINE UFUNCTION(BlueprintCallable) bool GetIsEnabled() const { return bIsEnabled; }
	FORCEINLINE UFUNCTION(BlueprintCallable) bool GetIsAvailable() const { return bIsAvailable; }
	FORCEINLINE UFUNCTION(BlueprintCallable) uint8 GetTeamID() const { return TeamID; }
	UFUNCTION(BlueprintCallable) FTransform GetStartTransform();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* StandParticleCT;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) 
	uint8 TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsEnabled;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;

	UPROPERTY()
	bool bIsAvailable;

	//

	UFUNCTION() void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
