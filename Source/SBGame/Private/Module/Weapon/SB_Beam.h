#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Beam.generated.h"

UCLASS()
class ASB_Beam : public AActor
{
	GENERATED_BODY()
	
public:	

	ASB_Beam();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* BeamParticle;

};
