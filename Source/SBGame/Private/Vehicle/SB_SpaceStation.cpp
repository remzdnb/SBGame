#include "Vehicle/SB_SpaceStation.h"
//
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

ASB_SpaceStation::ASB_SpaceStation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ACharacter::GetMovementComponent()->DestroyComponent();
}