#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Shield.generated.h"

UCLASS()
class ASB_Shield : public AActor
{
	GENERATED_BODY()
	
public:	

	ASB_Shield();

	virtual void BeginPlay() override;

	//

	UFUNCTION()
	void Init(const class ASB_DataManager* const NewDataManager);

	UFUNCTION()
	void OnSetupStart();

	UFUNCTION()
	void OnSetupStop();

	UFUNCTION(BlueprintNativeEvent)
	void OnDeployedBPN();

	UFUNCTION(BlueprintNativeEvent)
	void OnDestroyedBPN();

	//

	FORCEINLINE UFUNCTION() class UStaticMeshComponent* const GetMeshCT() const { return MeshCT; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshCT;

	//

	const class ASB_DataManager* DataManager;

	//


};
