#include "Campaign/SB_CampaignPlayerController.h"
#include "SB_BaseModule.h"
#include "SB_GameMode.h"
#include "RZ_UIManager.h"
//
#include "SB_DataManager.h"
#include "SB_Ship.h"
#include "SB_ShipCameraManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ASB_CampaignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (GMode)
		{
			GMode->QueryRespawn(this);
		}

		if (UIManager)
		{
			for (auto& MapRow : DataManager->UISettings.CampaignMenuWidgets)
			{
				UIManager->CreateMenuWidget(MapRow.Key, MapRow.Value);
			}

			UIManager->OnNewMenuWidgetSelected.AddUniqueDynamic(this, &ASB_CampaignPlayerController::OnNewMenuWidgetSelected);
		}

		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		bShowMouseCursor = true;
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
	}
}

void ASB_CampaignPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
	if (OwnedShip)
	{
		OwnedShip->GetShipCameraManager()->SetArmRotation(FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 145.0f, 0.0f), false);
		OwnedShip->GetShipCameraManager()->SetMaxTargetArmLength();
	}
}

void ASB_CampaignPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASB_CampaignPlayerController::OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget)
{
	if (WidgetName == "Home")
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this);

		if (OwnedShip)
		{
			OwnedShip->GetShipCameraManager()->SetArmRotation(FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 145.0f, 0.0f), true);
			OwnedShip->GetShipCameraManager()->SetMaxTargetArmLength();
		}
	}

	if (WidgetName == "Ship")
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);

		if (OwnedShip)
		{
			OwnedShip->GetShipCameraManager()->SetArmRotation(FRotator(-55.0f, OwnedShip->GetActorRotation().Yaw - 90.0f, 0.0f), true);
			OwnedShip->GetShipCameraManager()->SetMinTargetArmLength();
		}
	}

	if (GEngine)
	{
		const FString StringToPrint = "ASB_CampaignPlayerController::OnNewMenuWidgetSelected : Name " + WidgetName.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, StringToPrint);
	}
}

#pragma region +++++ Input ...

void ASB_CampaignPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ASB_CampaignPlayerController::OnLeftMouseButtonPressed).bConsumeInput = false;
}

void ASB_CampaignPlayerController::OnLeftMouseButtonPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectModule(OwnedShip->GetHoveredModule());
	}
}

#pragma endregion