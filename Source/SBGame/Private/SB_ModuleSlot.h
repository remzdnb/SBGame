#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SB_ModuleSlot.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_ModuleSlot : public USceneComponent
{
	GENERATED_BODY()

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:	

	USB_ModuleSlot();

	virtual void BeginPlay() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	void Init(const class ASB_DataManager* const NewDataManager);

	FORCEINLINE UFUNCTION() FName GetDataRowName() const { return DataRowName; }

private:

	const class ASB_DataManager* DataManager;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DisplayName;
};
