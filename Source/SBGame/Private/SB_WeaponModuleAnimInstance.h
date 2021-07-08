#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SB_WeaponModuleAnimInstance.generated.h"

UCLASS()
class USB_WeaponModuleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	FORCEINLINE UFUNCTION() void UpdateInstance(const FVector& NewOriginLocation, const FVector& NewTargetLocation)
	{
		OriginLocation = NewOriginLocation;
		TargetLocation = NewTargetLocation;
	}

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector OriginLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation;
};
