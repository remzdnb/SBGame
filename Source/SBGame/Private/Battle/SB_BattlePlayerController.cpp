// SBGame
#include "Battle/SB_BattlePlayerController.h"
#include "Battle/SB_BattleGameMode.h"
#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "Battle/SB_HUDMainWidget.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_ShipMovementComponent.h"
#include "SB_GameInstance.h"
#include "SB_GameSettings.h"
#include "SB_PlayerSaveGame.h"
// Plugins
#include "RZ_CameraActor.h"
#include "RZ_UIManager.h"
#include "RZ_MenuLayoutWidget.h"
#include "RZ_DamageMarkerWidget.h"
#include "RZ_LogWidget.h"
// Engine
#include "SB_FSGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void ASB_BattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	GMode = Cast<ASB_BattleGameMode>(GetWorld()->GetAuthGameMode());
	GState = GetWorld()->GetGameState<ASB_GameState>();
	PState = Cast<ASB_PlayerState>(PlayerState);

	if (IsLocalPlayerController())
	{
		//

		PState->SetName(true, GInstance->GetSaveGame()->PlayerName);
		
		// Spawn outline post process.
		
		const FActorSpawnParameters SpawnParameters;
		GetWorld()->SpawnActor<AActor>(GInstance->GameSettings->BattlePostProcess_BP, SpawnParameters);

		// Init UI

		USB_HUDMainWidget* BattleHUDWidget = CreateWidget<USB_HUDMainWidget>(this, GInstance->UISettings.HUDMain_WBP);
		UIManager->AddHUDWidget(BattleHUDWidget);
		//LogWidget = CreateWidget<URZ_LogWidget>(this, GInstance->UISettings.Log_WBP);
		//UIManager->AddHUDWidget(LogWidget);
		for (auto& MapRow : GInstance->UISettings.BattleMenuWidgets)
		{
			UIManager->GetMenuLayoutWidget()->CreateMenuWidget(MapRow.Key, MapRow.Value);
		}
		UIManager->ToggleHUD(true);

		// Init controller settings
	
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		bShowMouseCursor = true;
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
		HitResultTraceDistance = 1000000.0f;
	}
}

void ASB_BattlePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHoveredVehicle();
	//UpdateViewTarget(DeltaTime);
}

void ASB_BattlePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (OwnedVehicle)
	{
		CameraActor->SetNewTargetActor(OwnedVehicle, FVector(0.0f, 0.0f, 5000.0f));
		//OwnedVehicle->OnDestroyed.AddUniqueDynamic(this, &ASB_BattlePlayerController::OnVehicleDestroyed);
		//OwnedVehicle->GetShipCameraManager()->SetArmRotation(FRotator(-25.0f, OwnedVehicle->GetActorRotation().Yaw - 145.0f, 0.0f), false);
		//OwnedVehicle->GetShipCameraManager()->SetMaxTargetArmLength();
	}

	SetViewTargetWithBlend(CameraActor, 0.0f);
}

void ASB_BattlePlayerController::Respawn_Server_Implementation()
{
	if (OwnedVehicle)
	{
		if (OwnedVehicle->GetState() == ESB_ShipState::Destroyed)
		{
			UnPossess();
			OwnedVehicle->Destroy();
			OwnedVehicle = nullptr;
			//GMode->QueryRespawn(this);
		}
	}
}

ASB_Vehicle* const ASB_BattlePlayerController::SpawnAndPossessVehicle(const FTransform& SpawnTransform)
{
	if (GetLocalRole() < ROLE_Authority || GetPawn() != nullptr)
		return nullptr;

	const FSB_VehicleData* const VehicleData = GInstance->GetVehicleDataFromRow(GInstance->GetSaveGame()->VehicleDataRowName);
	if (VehicleData)
	{
		ASB_Vehicle* NewShip = GetWorld()->SpawnActorDeferred<ASB_Vehicle>(
			VehicleData->Vehicle_BP, SpawnTransform,
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
		if (NewShip)
		{
			UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
			OnPossess(NewShip);
			NewShip->LoadConfig(GInstance->GetSaveGame()->ShipConfig, false);
			NewShip->OnDestroyed.AddDynamic(this, &ASB_BattlePlayerController::OnVehicleDestroyed);
			OnRep_Pawn();
			
			return NewShip;
		}
	}

	return nullptr;
}

uint8 ASB_BattlePlayerController::GetTeamID()
{
	if (PState)
	{
		return PState->GetTeamID();
	}

	return 0;
}

void ASB_BattlePlayerController::JoinTeam(uint8 TeamID)
{
	if (GState)
	{
		GState->AddPlayerToTeam(this, TeamID);
	}
}

#pragma region +++++ Vehicle ...

void ASB_BattlePlayerController::OnDamageDealt(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType)
{
	OnDamageDealt_Client(PrimaryDamage, SecondaryDamage, HitLocation, PrimaryDamageType);
}

void ASB_BattlePlayerController::OnDamageDealt_Client_Implementation(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType)
{
	FLinearColor NewColor = FLinearColor::White;
	if (PrimaryDamageType == ESB_PrimaryDamageType::Ship)
		NewColor = FLinearColor::Red;
	if (PrimaryDamageType == ESB_PrimaryDamageType::Shield)
		NewColor = FLinearColor::Blue;
	
	URZ_DamageMarkerWidget* const NewDamageMarker = CreateWidget<URZ_DamageMarkerWidget>(GetWorld(), GInstance->UISettings.HUDDamageMarker_WBP);
	if (NewDamageMarker)
	{
		NewDamageMarker->Init(PrimaryDamage, SecondaryDamage, HitLocation, NewColor);
		UIManager->AddHUDWidget(NewDamageMarker, false);
	}
}

void ASB_BattlePlayerController::OnVehicleDestroyed(AActor* DestroyedVehicle)
{
	SetViewTargetWithBlend(CameraActor, 0.0f);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "VehicleDestroyed BattlePC");
}

#pragma endregion

#pragma region +++++ Input ...

void ASB_BattlePlayerController::UpdateHoveredVehicle()
{
	if (IsLocalPlayerController() == false)
		return;
	
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	ASB_Vehicle* const NewHoveredVehicle = Cast<ASB_Vehicle>(HitResult.Actor);
	if (NewHoveredVehicle != HoveredVehicle &&
		NewHoveredVehicle != GetPawn())
	{
		if (HoveredVehicle.IsValid() && HoveredVehicle != SelectedVehicle)
			HoveredVehicle->ToggleOutline(false);

		if (NewHoveredVehicle && NewHoveredVehicle != SelectedVehicle)
			NewHoveredVehicle->ToggleOutline(true, 1);

		HoveredVehicle = NewHoveredVehicle;
	}

	if (GInstance->DebugSettings.bIsDebugEnabled_PlayerController)
	{
		FString StringToPrint;
		if (HoveredVehicle.IsValid())
			StringToPrint = "ASB_BattlePlayerController::UpdateHoveredVehicle - Hovered : " + HoveredVehicle->GetName();
		else
			StringToPrint = "ASB_BattlePlayerController::UpdateHoveredVehicle - Hovered : nullptr";
		
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::White, StringToPrint);
	}
}

void ASB_BattlePlayerController::SelectHoveredVehicle()
{
	if (HoveredVehicle == SelectedVehicle)
		return;
	
	if (SelectedVehicle.IsValid())
	{
		SelectedVehicle->ToggleOutline(false);
		OwnedVehicle->SetPriorityTarget(nullptr);
		SelectedVehicle = nullptr;
	}
	
	if (HoveredVehicle.IsValid())
	{
		HoveredVehicle->ToggleOutline(true, 2);
		OwnedVehicle->SetPriorityTarget(HoveredVehicle.Get());
		SelectedVehicle = HoveredVehicle;
	}
}

void ASB_BattlePlayerController::UpdateViewTarget(float DeltaTime) const
{
	if (IsLocalPlayerController() == false)
		return;

	const FVector Start = PlayerCameraManager->GetCameraLocation();
	const FVector End = Start + (PlayerCameraManager->GetCameraRotation().Vector() * 9999999999.0f);
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bIgnoreTouches = true;
	TraceParams.AddIgnoredActor(OwnedVehicle);
	FHitResult Hit;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	if (Hit.Actor.IsValid())
	{
		if (OwnedVehicle)
		{
			OwnedVehicle->UpdateOwnerViewData(Hit.Location, Hit.Actor.Get());
		}

		/*if (DataManager->GameSettings.bIsDebugEnabled_PlayerController)
		{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, *("ASB_PlayerController::UpdateViewTarget // Actor : " + Hit.Actor->GetName() + " // Component : " + Hit.Component->GetName()));
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.Location, 500.0f, 10, FColor::Green, DeltaTime + 0.01f, 20.0f);
		}*/
	}
}

void ASB_BattlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ASB_BattlePlayerController::MoveForwardAxis).bConsumeInput = false;
	InputComponent->BindAxis("MoveRight", this, &ASB_BattlePlayerController::MoveRightAxis).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ASB_BattlePlayerController::LeftMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ASB_BattlePlayerController::LeftMouseButtonReleased).bConsumeInput = false;
	InputComponent->BindAction("Tab", IE_Pressed, this, &ASB_BattlePlayerController::TabKeyPressed).bConsumeInput = false;
}

void ASB_BattlePlayerController::MoveForwardAxis(float AxisValue)
{
	if (OwnedVehicle && OwnedVehicle == GetPawn())
	{
		if (OwnedVehicle->GetShipMovement())
		{
			OwnedVehicle->GetShipMovement()->MoveForward(AxisValue);
		}
	}
	else
	{
		if (CameraActor)
		{
			CameraActor->MoveForward(AxisValue);
		}
	}
}

void ASB_BattlePlayerController::MoveRightAxis(float AxisValue)
{
	if (OwnedVehicle && OwnedVehicle == GetPawn())
	{
		if (OwnedVehicle->GetShipMovement())
		{
			OwnedVehicle->GetShipMovement()->TurnRight(AxisValue);
		}
	}
	else
	{
		if (CameraActor)
		{
			CameraActor->MoveRight(AxisValue);
		}
	}
}

void ASB_BattlePlayerController::LeftMouseButtonPressed()
{
	if (OwnedVehicle)
	{
		SelectHoveredVehicle();
	}
}

void ASB_BattlePlayerController::LeftMouseButtonReleased()
{
}

void ASB_BattlePlayerController::TabKeyPressed()
{
	if (UIManager)
	{
		if (UIManager->IsMenuOpen())
		{
			UIManager->ToggleMenu(false);
			UIManager->ToggleHUD(true);

			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
			bShowMouseCursor = false;
		}
		else
		{
			UIManager->ToggleMenu(true);
			UIManager->ToggleHUD(false);

			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
			bShowMouseCursor = true;
		}
	}
}

#pragma endregion
