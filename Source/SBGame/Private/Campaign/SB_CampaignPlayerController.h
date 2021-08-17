///// SB_CampaignPlayerController.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_PlayerController.h"
//
#include "CoreMinimal.h"
#include "SB_CampaignPlayerController.generated.h"

#define DEFAULT_CAMERAARMLENGTH 50000.0f
#define DEFAULT_CAMERAARMROTATIONPITCH -20.0f
#define DEFAULT_CAMERAARMROTATIONYAW -145.0f
#define SHIPCONFIG_CAMERAARMLENGTH 30000.0f
#define SHIPCONFIG_CAMERAARMROTATIONPITCH -25.0f
#define SHIPCONFIG_CAMERAARMROTATIONYAW -90.0f

UCLASS()
class ASB_CampaignPlayerController : public ASB_PlayerController
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void SelectShip(const FName& NewShipDataRowName);

private:
	
	UFUNCTION()
	void OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget);

	//

	UPROPERTY()
	FName OpenedWidgetName;


///// Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void SetupInputComponent() override;

private:

	UFUNCTION()
	void OnLeftMouseButtonPressed();
};
