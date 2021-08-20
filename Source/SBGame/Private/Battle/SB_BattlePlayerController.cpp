// SBGame
#include "Battle/SB_BattlePlayerController.h"
#include "Battle/SB_BattleGameMode.h"
#include "Battle/SB_GameState.h"
#include "Battle/SB_PlayerState.h"
#include "Battle/SB_HUDMainWidget.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
// Plugins
#include "RZ_CameraActor.h"
#include "RZ_UIManager.h"
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
	GState = Cast<ASB_GameState>(GetWorld()->GetGameState());
	PState = Cast<ASB_PlayerState>(PlayerState);

	USB_HUDMainWidget* BattleHUDWidget = CreateWidget<USB_HUDMainWidget>(this, GInstance->UISettings.HUDMain_WBP);
	UIManager->AddHUDWidget(BattleHUDWidget);
	//LogWidget = CreateWidget<URZ_LogWidget>(this, GInstance->UISettings.Log_WBP);
	//UIManager->AddHUDWidget(LogWidget);
	UIManager->ToggleHUD(true);
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	bShowMouseCursor = false;
	bEnableMouseOverEvents = false;
	bEnableClickEvents = false;
}

void ASB_BattlePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateViewTarget(DeltaTime);
}

void ASB_BattlePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (OwnedShip)
	{
		CameraActor->SetNewTargetActor(OwnedShip, FVector(0.0f, 0.0f, 5000.0f));
		SetViewTargetWithBlend(CameraActor, 0.0f);
		//OwnedShip->GetShipCameraManager()->SetArmRotation(FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 145.0f, 0.0f), false);
		//OwnedShip->GetShipCameraManager()->SetMaxTargetArmLength();
	}
}


void ASB_BattlePlayerController::Respawn_Server_Implementation()
{
	if (OwnedShip)
	{
		if (OwnedShip->GetState() == ESB_ShipState::Destroyed)
		{
			UnPossess();
			OwnedShip->Destroy();
			OwnedShip = nullptr;
			//GMode->QueryRespawn(this);
		}
	}
}

ASB_Ship* const ASB_BattlePlayerController::SpawnAndPossessVehicle(const FTransform& SpawnTransform)
{
	if (GetLocalRole() < ROLE_Authority || GetPawn() != nullptr)
		return nullptr;

	const FSB_ShipData* const ShipData = GInstance->GetShipDataFromRow(GInstance->GetSaveGame()->ShipDataRowName);
	if (ShipData)
	{
		ASB_Ship* NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(
			ShipData->ShipBP, SpawnTransform,
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
		if (NewShip)
		{
			UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
			OnPossess(NewShip);
			NewShip->LoadConfig(GInstance->GetSaveGame()->ShipConfig, false);
			OnRep_Pawn();
			
			return NewShip;
		}
	}

	return nullptr;
}


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
	
	/*URZ_DamageMarkerWidget* const NewDamageMarker = CreateWidget<URZ_DamageMarkerWidget>(GetWorld(), DataManager->UISettings.DamageMarker_WBP);
	if (NewDamageMarker)
	{
	NewDamageMarker->Init(PrimaryDamage, SecondaryDamage, HitLocation, NewColor);
	UIManager->AddHUDWidget(NewDamageMarker);
	}*/
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
	TraceParams.AddIgnoredActor(OwnedShip);
	FHitResult Hit;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	if (Hit.Actor.IsValid())
	{
		if (OwnedShip)
		{
			OwnedShip->UpdateOwnerViewData(Hit.Location, Hit.Actor.Get());
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

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ASB_BattlePlayerController::LeftMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ASB_BattlePlayerController::LeftMouseButtonReleased).bConsumeInput = false;
	InputComponent->BindAction("Tab", IE_Pressed, this, &ASB_BattlePlayerController::TabKeyPressed).bConsumeInput = false;
}

uint8 ASB_BattlePlayerController::GetTeamID()
{
	if (PState)
	{
		return PState->GetTeam();
	}

	return 0;
}

void ASB_BattlePlayerController::LeftMouseButtonPressed()
{
	if (OwnedShip)
	{
		/*if (OwnedShip->ShieldModule->GetIsSetupMode() == 1)
		{
			OwnedShip->ShieldModule->Deploy();
		}*/

			OwnedShip->StartFireSelectedWeapons();

	}
}

void ASB_BattlePlayerController::LeftMouseButtonReleased()
{
	if (OwnedShip)
	{
		OwnedShip->StopFireAllWeapons();
	}
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

