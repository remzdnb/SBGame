// SBGame
#include "Campaign/SB_CampaignPlayerController.h"
#include "Vehicle/SB_Vehicle.h"
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
		SelectShip(GInstance->GetSaveGame()->VehicleDataRowName);

		// Camera

		CameraActor->SetNewLocation(OwnedVehicle->GetActorLocation(), true);
		CameraActor->SetNewRotation(FRotator(
										DEFAULT_CAMERAARMROTATIONPITCH,
										OwnedVehicle->GetActorRotation().Yaw + DEFAULT_CAMERAARMROTATIONYAW,
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

void ASB_CampaignPlayerController::SelectShip(const FName& NewVehicleDataRowName)
{
	const FSB_VehicleData* const NewVehicleData = GInstance->GetVehicleDataFromRow(NewVehicleDataRowName);
	if (NewVehicleData)
	{
		if (OwnedVehicle)
		{
			UnPossess();
			OwnedVehicle->Destroy();
		}
				
		const FTransform SpawnTransform = FTransform(FVector::ZeroVector);
		ASB_Vehicle* NewShip = GetWorld()->SpawnActorDeferred<ASB_Vehicle>(NewVehicleData->Vehicle_BP, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (NewShip)
		{
			UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
			OnPossess(NewShip);
			OwnedVehicle = Cast<ASB_Vehicle>(GetPawn());
			OwnedVehicle->LoadConfig(GInstance->GetSaveGame()->ShipConfig, true);
			OnNewOwnedVehicle.Broadcast(OwnedVehicle);

			SetViewTargetWithBlend(CameraActor, 0.0f);
		}

		GInstance->SaveSelectedShip(NewVehicleDataRowName);
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
		
		CameraActor->SetNewLocation(OwnedVehicle->GetActorLocation(), true);
		CameraActor->SetNewArmLength(SHIPCONFIG_CAMERAARMLENGTH, false);
		CameraActor->SetNewRotation(FRotator(
										SHIPCONFIG_CAMERAARMROTATIONPITCH,
										OwnedVehicle->GetActorRotation().Yaw + SHIPCONFIG_CAMERAARMROTATIONYAW,
										0.0f),
									true
		);
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this);
		bEnableMouseOverEvents = false;
		bEnableClickEvents = false;

		CameraActor->SetNewLocation(OwnedVehicle->GetActorLocation(), true);
		CameraActor->SetNewArmLength(DEFAULT_CAMERAARMLENGTH, false);
		CameraActor->SetNewRotation(FRotator(
										DEFAULT_CAMERAARMROTATIONPITCH,
										OwnedVehicle->GetActorRotation().Yaw + DEFAULT_CAMERAARMROTATIONYAW,
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
	if (OwnedVehicle)
	{
		//OwnedVehicle->SelectModule(OwnedVehicle->GetHoveredModule());
	}
}

#pragma endregion