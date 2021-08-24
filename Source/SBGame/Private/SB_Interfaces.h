#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SB_Interfaces.generated.h"

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
	virtual class ASB_Vehicle* const SpawnAndPossessVehicle(const FTransform& SpawnTransform) = 0;
	virtual void OnVehicleDestroyed(AActor* const DestroyedVehicle) = 0;
};

//

UINTERFACE(MinimalAPI)
class USB_TeamInterface : public UInterface
{
	GENERATED_BODY()
};

class ISB_TeamInterface
{
	GENERATED_BODY()

public:

	virtual uint8 GetTeamID() = 0;
};

//

UINTERFACE(MinimalAPI)
class USB_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class ISB_CombatInterface // damage interface
{
	GENERATED_BODY()

public:

	virtual void ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController) = 0;
};