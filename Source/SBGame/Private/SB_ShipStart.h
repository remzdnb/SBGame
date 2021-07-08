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

///// Components
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* StandParticleCT;

	UFUNCTION() void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

///// Actor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;

///// SB_ShipStart
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE UFUNCTION(BlueprintCallable) bool GetIsEnabled() const { return bIsEnabled; }
	FORCEINLINE UFUNCTION(BlueprintCallable) bool GetIsAvailable() const { return bIsAvailable; }
	FORCEINLINE UFUNCTION(BlueprintCallable) uint8 GetTeamID() const { return TeamID; }
	UFUNCTION(BlueprintCallable) FTransform GetStartTransform();

	UFUNCTION(NetMulticast, Reliable)
	void UpdateAvailability_Multicast(bool bNewIsAvailable);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAvailability_BPI(bool bNewIsAvailable);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) 
	uint8 TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsEnabled;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;

	UPROPERTY()
	bool bIsAvailable;
};
