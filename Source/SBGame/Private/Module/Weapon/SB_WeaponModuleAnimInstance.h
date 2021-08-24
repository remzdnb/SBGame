#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SB_WeaponModuleAnimInstance.generated.h"

UCLASS()
class USB_WeaponModuleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	FORCEINLINE UFUNCTION() void UpdateInstance(const FRotator& NewTargetRotation)
	{
		TargetRotation = NewTargetRotation;
	}

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator TargetRotation;
};
