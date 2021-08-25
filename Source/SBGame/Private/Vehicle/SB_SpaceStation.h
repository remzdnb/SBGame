#pragma once

#include "CoreMinimal.h"
#include "Vehicle/SB_Vehicle.h"
#include "SB_SpaceStation.generated.h"

UCLASS()
class ASB_SpaceStation : public ASB_Vehicle
{
	GENERATED_BODY()

public:

	ASB_SpaceStation(const FObjectInitializer& ObjectInitializer);
};
