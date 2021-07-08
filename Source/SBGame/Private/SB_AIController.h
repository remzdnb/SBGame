///// SB_AIController.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SB_AIController.generated.h"

UCLASS()
class ASB_AIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASB_Ship* OwnedShip;
	
};
