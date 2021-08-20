// SBGame
#include "Campaign/SB_CampaignPlayerController.h"
#include "Ship/SB_Ship.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
// Plugins
#include "RZ_UIManager.h"
#include "RZ_MenuLayoutWidget.h"
#include "RZ_CameraActor.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Module/SB_ModuleSlotComponent.h"

void ASB_CampaignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		SelectShip(GInstance->GetSaveGame()->ShipDataRowName);

		// Camera

		CameraActor->SetNewLocation(OwnedShip->GetActorLocation(), true);
		CameraActor->SetNewRotation(FRotator(
										DEFAULT_CAMERAARMROTATIONPITCH,
										OwnedShip->GetActorRotation().Yaw + DEFAULT_CAMERAARMROTATIONYAW,
										0.0f),
									false
		);
		CameraActor->SetNewArmLength(DEFAULT_CAMERAARMLENGTH, false);
		
		// UI
		
		for (auto& MapRow : GInstance->UISettings.CampaignMenuWidgets)
		{
			UIManager->GetMenuLayoutWidget()->CreateMenuWidget(MapRow.Key, MapRow.Value);
		}

		UIManager->OnNewMenuWidgetSelected.AddUniqueDynamic(this, &ASB_CampaignPlayerController::OnNewMenuWidgetSelected);
		UIManager->ToggleMenu(true);

		// Controls
		
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this);
		bShowMouseCursor = true;
		bEnableMouseOverEvents = false;
		bEnableClickEvents = false;
	}
}

void ASB_CampaignPlayerController::SelectShip(const FName& NewShipDataRowName)
{
	const FSB_ShipData* const NewShipData = GInstance->GetShipDataFromRow(NewShipDataRowName);
	if (NewShipData)
	{
		if (OwnedShip)
		{
			UnPossess();
			OwnedShip->Destroy();
		}
				
		const FTransform SpawnTransform = FTransform(FVector::ZeroVector);
		ASB_Ship* NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(NewShipData->ShipBP, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (NewShip)
		{
			UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
			OnPossess(NewShip);
			OwnedShip = Cast<ASB_Ship>(GetPawn());
			OwnedShip->LoadConfig(GInstance->GetSaveGame()->ShipConfig, true);
			OnNewOwnedShip.Broadcast(OwnedShip);

			SetViewTargetWithBlend(CameraActor, 0.0f);
		}

		GInstance->SaveSelectedShip(NewShipDataRowName);
	}
}

void ASB_CampaignPlayerController::OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget)
{
	OpenedWidgetName = WidgetName;
	
	if (WidgetName == "Ship")
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
		
		CameraActor->SetNewLocation(OwnedShip->GetActorLocation(), true);
		CameraActor->SetNewArmLength(SHIPCONFIG_CAMERAARMLENGTH, false);
		CameraActor->SetNewRotation(FRotator(
										SHIPCONFIG_CAMERAARMROTATIONPITCH,
										OwnedShip->GetActorRotation().Yaw + SHIPCONFIG_CAMERAARMROTATIONYAW,
										0.0f),
									true
		);
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this);
		bEnableMouseOverEvents = false;
		bEnableClickEvents = false;

		CameraActor->SetNewLocation(OwnedShip->GetActorLocation(), true);
		CameraActor->SetNewArmLength(DEFAULT_CAMERAARMLENGTH, false);
		CameraActor->SetNewRotation(FRotator(
										DEFAULT_CAMERAARMROTATIONPITCH,
										OwnedShip->GetActorRotation().Yaw + DEFAULT_CAMERAARMROTATIONYAW,
										0.0f),
									true
		);
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
		//OwnedShip->SelectModule(OwnedShip->GetHoveredModule());
	}
}

#pragma endregion