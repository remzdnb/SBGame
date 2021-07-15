#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SB_Interfaces.generated.h"

UINTERFACE(MinimalAPI)
class USB_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class ISB_CombatInterface
{
	GENERATED_BODY()

public:

	virtual void ApplyDamage(const float Damage, const FVector& HitLocation, AController* const InstigatorController) = 0;
};
