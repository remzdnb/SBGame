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

	virtual void ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController) = 0;
};

//

UINTERFACE(MinimalAPI)
class USB_ControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class ISB_ControllerInterface
{
	GENERATED_BODY()

public:

	virtual uint8 GetTeamID() = 0;
	virtual class ASB_Ship* const SpawnAndPossessVehicle(const FTransform& SpawnTransform) = 0;
};

