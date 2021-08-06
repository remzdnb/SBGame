#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_ModuleSlotActor.generated.h"

UCLASS()
class ASB_ModuleSlotActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	ASB_ModuleSlotActor();

	virtual void BeginPlay() override;

	//

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* DemoMesh;

};
