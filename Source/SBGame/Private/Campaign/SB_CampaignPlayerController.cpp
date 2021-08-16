// SBGame
#include "Campaign/SB_CampaignPlayerController.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameMode.h"
#include "SB_DataManager.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipCameraManager.h"
#include "SB_PlayerSaveGame.h"
// UIPlugin
#include "RZ_UIManager.h"
#include "RZ_MenuLayoutWidget.h"
// Engine
#include "SB_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ASB_CampaignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// Ship
		
		const FSB_ShipData* const ShipData = GInstance->GetShipDataFromRow(GInstance->GetSaveGame()->ShipDataRowName);
		if (ShipData)
		{
			const FTransform SpawnTransform = FTransform(FVector::ZeroVector);
			ASB_Ship* NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(ShipData->ShipBP, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (NewShip)
			{
				UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
				OnPossess(NewShip);
				OnRep_Pawn();
			}
		}

		// UI
		
		for (auto& MapRow : GInstance->UISettings.CampaignMenuWidgets)
		{
			UIManager->GetMenuLayoutWidget()->CreateMenuWidget(MapRow.Key, MapRow.Value);
		}

		UIManager->OnNewMenuWidgetSelected.AddUniqueDynamic(this, &ASB_CampaignPlayerController::OnNewMenuWidgetSelected);
		UIManager->ToggleMenu(true);

		// Controller
		
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

void ASB_CampaignPlayerController::SelectShip(const FName& NewShipDataRowName)
{
	UE_LOG(LogTemp, Log, TEXT("ASB_CampaignPlayerController::NewShipDataRowName : %s"), *NewShipDataRowName.ToString());
	
	const FSB_ShipData* const NewShipData = GInstance->GetShipDataFromRow(NewShipDataRowName);
	if (NewShipData)
	{
		if (OwnedShip)
		{
			UnPossess();
			OwnedShip->Destroy();
		}

		GInstance->SaveSelectedShip(NewShipDataRowName);
		
		const FTransform SpawnTransform = FTransform(FVector::ZeroVector);
		ASB_Ship* NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(NewShipData->ShipBP, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (NewShip)
		{
			UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
			OnPossess(NewShip);
			OnRep_Pawn();
		}
	}
}

void ASB_CampaignPlayerController::OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget)
{
	if (WidgetName == "Ship")
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);

		if (OwnedShip)
		{
			OwnedShip->GetShipCameraManager()->SetArmRotation(FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 110.0f, 0.0f), true);
			OwnedShip->GetShipCameraManager()->SetMinTargetArmLength();
		}

		//bEnableMouseOverEvents = true;
		//bEnableClickEvents = true;
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this);

		if (OwnedShip)
		{
			//OwnedShip->SelectModule(nullptr);
			OwnedShip->GetShipCameraManager()->SetArmRotation(FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 145.0f, 0.0f), true);
			OwnedShip->GetShipCameraManager()->SetMaxTargetArmLength();
		}
		
		//bEnableMouseOverEvents = false;
		//bEnableClickEvents = false;
	}

	/*if (GEngine)
	{
		const FString StringToPrint = "ASB_CampaignPlayerController::OnNewMenuWidgetSelected : Name " + WidgetName.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, StringToPrint);
	}*/
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