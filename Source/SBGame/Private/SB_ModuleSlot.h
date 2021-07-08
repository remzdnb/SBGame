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

	FORCEINLINE UFUNCTION() FName GetSlotName() const { return SlotName; }
	FORCEINLINE UFUNCTION() FName GetModuleName() const { return ModuleName; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ModuleName;
};
