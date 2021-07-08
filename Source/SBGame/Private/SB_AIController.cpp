#include "SB_AIController.h"
#include "SB_Ship.h"

void ASB_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASB_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnedShip = Cast<ASB_Ship>(InPawn);
}
