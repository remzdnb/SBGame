///// SB_CampaignPlayerController.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "SB_PlayerController.h"
#include "CoreMinimal.h"
#include "SB_CampaignPlayerController.generated.h"

UCLASS()
class ASB_CampaignPlayerController : public ASB_PlayerController
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;

	UFUNCTION()
	void SelectShip(const FName& NewShipDataRowName);

private:
	
	UFUNCTION()
	void OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget);


///// Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void SetupInputComponent() override;

private:

	UFUNCTION()
	void OnLeftMouseButtonPressed();
};
