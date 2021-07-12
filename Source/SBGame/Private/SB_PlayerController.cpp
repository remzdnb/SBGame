#include "SB_PlayerController.h"
#include "SB_PlayerState.h"
#include "SB_GameMode.h"
#include "SB_GameState.h"
#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_ShipCameraManager.h"
#include "SB_ShipOTMWidget.h"
#include "SB_ShieldModule.h"
#include "SB_SpectatorPawn.h"
#include "SB_UIManager.h"
#include "SB_BattleMenuWidget.h"
#include "SB_BattleHUDWidget.h"
#include "SB_DataManager.h"
#include "RZ_DamageMarkerWidget.h"
//
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DrawDebugHelpers.h"
//
#include "GameFramework/SpectatorPawn.h"

void ASB_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	/*for (TActorIterator<ADefaultPawn> NewSpectatorPawn(GetWorld()); NewSpectatorPawn; ++NewSpectatorPawn)
	{
		OwnedSpectatorPawn = *NewSpectatorPawn;
		break;
	}*/

	GMode = Cast<ASB_GameMode>(GetWorld()->GetAuthGameMode());
	PState = Cast<ASB_PlayerState>(PlayerState);

	//PState->SetName();
}

void ASB_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// UI setup
		UIManager = Cast<ASB_UIManager>(GetHUD());
		if (UIManager)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
			bShowMouseCursor = false;
		}

		// Spectator setup
		/*OwnedSpectatorPawn = GetWorld()->SpawnActorDeferred<ASB_SpectatorPawn>(ASB_SpectatorPawn::StaticClass(), FTransform(FVector()), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (OwnedSpectatorPawn)
		{
			UGameplayStatics::FinishSpawningActor(OwnedSpectatorPawn, FTransform(FVector()));
		}*/

		//PState->SetName();
		//PState->SetTeamID(1);
	}

	// Start player
	if (DataManager->GameSettings.bAutoSpawnPlayers)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			GMode->QueryRespawn(this);
		}
	}
	else
	{
		if (IsLocalPlayerController())
		{
			SetViewTargetWithBlend(OwnedSpectatorPawn);
			//Possess(OwnedSpectatorPawn);
		}
	}

	//SetControlRotation(FRotator(0.0f, OwnedShip->GetShipMovementCT()->GetTargetRotationYaw(), 0.0f));
}

void ASB_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnedShip)
	{
		if (OwnedShip->GetShipMovementCT())
		{
			float LerpedYaw = FMath::Lerp(GetControlRotation().Yaw, OwnedShip->GetShipMovementCT()->GetTargetRotationYaw(), DataManager->ShipSettings.TurnInertia);
			SetControlRotation(FRotator(0.0f, LerpedYaw, 0.0f));
		}
	}

	UpdateViewTarget();
}

void ASB_PlayerController::UpdateViewTarget()
{
	if (IsLocalPlayerController() == false)
		return;

	const FVector Start = PlayerCameraManager->GetCameraLocation();
	const FVector End = Start + (PlayerCameraManager->GetCameraRotation().Vector() * 10000000.0f);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnedShip);
	TArray<FHitResult> Hits;

	GetWorld()->LineTraceMultiByChannel(Hits, Start, End, ECC_Visibility, TraceParams);
	for (FHitResult& Hit : Hits)
	{
		if (Hit.Actor.IsValid() && Hit.Actor != GetPawn())
		{
			if (OwnedShip)
			{
				OwnedShip->UpdateOwnerViewData(/*GetControlRotation(), */Hit.Location, Hit.Actor.Get());

				//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.Location, 500.0f, 10, FColor::Green, 0.1f, 20.0f);
			}

			break;
		}
	}
}

void ASB_PlayerController::SpawnAndPossessShip(const FTransform& SpawnTransform)
{
	if (GetLocalRole() < ROLE_Authority || GetPawn() != nullptr)
		return;

	ASB_Ship* NewShip = GetWorld()->SpawnActorDeferred<ASB_Ship>(DataManager->GameSettings.ShipClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (NewShip)
	{
		UGameplayStatics::FinishSpawningActor(NewShip, SpawnTransform);
		OnPossess(NewShip);
		OnRep_Pawn();
	}
}

void ASB_PlayerController::OnDamageDealt(const FVector& HitLocation, float Damage)
{
	OnDamageDealt_Client(HitLocation, Damage);
}

void ASB_PlayerController::OnDamageDealt_Client_Implementation(const FVector& HitLocation, float Damage)
{
	URZ_DamageMarkerWidget* const NewDamageMarker = CreateWidget<URZ_DamageMarkerWidget>(GetWorld(), DataManager->UISettings.DamageMarker_WBP);
	if (NewDamageMarker)
	{
		NewDamageMarker->Init(HitLocation, Damage);
		UIManager->AddHUDWidget(NewDamageMarker);
	}
}

void ASB_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OwnedShip = Cast<ASB_Ship>(GetPawn());
	if (OwnedShip)
	{
		OwnedShip->DestroyedEvent.AddUniqueDynamic(this, &ASB_PlayerController::OnOwnedShipDestroyed);
		OnNewOwnedShip.Broadcast(OwnedShip);
		SetControlRotation(FRotator(-45.0f, OwnedShip->GetActorRotation().Yaw, 0.0f));
	}
}

void ASB_PlayerController::OnOwnedShipDestroyed(const APlayerState* const InstigatorPS)
{
}

void ASB_PlayerController::Respawn_Server_Implementation()
{
	if (OwnedShip)
	{
		if (OwnedShip->GetState() == ESB_ShipState::Destroyed)
		{
			UnPossess();
			OwnedShip->Destroy();
			OwnedShip = nullptr;
			GMode->QueryRespawn(this);
		}
	}
}

#pragma region +++++ Input ...

void ASB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis("LookUp", this, &ASB_PlayerController::LookUpAxis).bConsumeInput = false;
	InputComponent->BindAxis("LookRight", this, &ASB_PlayerController::LookRightAxis).bConsumeInput = false;
	InputComponent->BindAxis("MoveForward", this, &ASB_PlayerController::MoveForwardAxis).bConsumeInput = false;
	InputComponent->BindAxis("MoveRight", this, &ASB_PlayerController::MoveRightAxis).bConsumeInput = false;
	InputComponent->BindAxis("MouseWheel", this, &ASB_PlayerController::MouseWheelAxis).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ASB_PlayerController::LeftMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ASB_PlayerController::LeftMouseButtonReleased).bConsumeInput = false;
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ASB_PlayerController::RightMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ASB_PlayerController::RightMouseButtonReleased).bConsumeInput = false;
	InputComponent->BindAction("Tab", IE_Pressed, this, &ASB_PlayerController::TabKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("Shift", IE_Pressed, this, &ASB_PlayerController::ShiftKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("Shift", IE_Released, this, &ASB_PlayerController::ShiftKeyReleased).bConsumeInput = false;
	InputComponent->BindAction("SpaceBar", IE_Pressed, this, &ASB_PlayerController::SpaceBarKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SelectWeapon1", IE_Pressed, this, &ASB_PlayerController::SelectWeapon1KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SelectWeapon2", IE_Pressed, this, &ASB_PlayerController::SelectWeapon2KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SelectWeapon3", IE_Pressed, this, &ASB_PlayerController::SelectWeapon3KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SelectWeapon4", IE_Pressed, this, &ASB_PlayerController::SelectWeapon4KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SelectWeapon5", IE_Pressed, this, &ASB_PlayerController::SelectWeapon5KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SelectAllWeapons", IE_Pressed, this, &ASB_PlayerController::SelectAllWeaponsKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("UnselectAllWeapons", IE_Pressed, this, &ASB_PlayerController::UnselectAllWeaponsKeyPressed).bConsumeInput = false;
}

void ASB_PlayerController::LookUpAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraManager())
		{
			OwnedShip->GetShipCameraManager()->AddPitchInput(AxisValue);
		}
	}
}

void ASB_PlayerController::LookRightAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraManager())
		{
			OwnedShip->GetShipCameraManager()->AddYawInput(AxisValue);
		}
	}
}

void ASB_PlayerController::MoveForwardAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		OwnedShip->GetShipMovementCT()->MoveForward(AxisValue);
	}
}

void ASB_PlayerController::MoveRightAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		OwnedShip->GetShipMovementCT()->TurnRight(AxisValue);
	}
}

void ASB_PlayerController::MouseWheelAxis(float AxisValue)
{
	if (AxisValue > 0)
	{
		if (OwnedShip && OwnedShip == GetPawn())
		{
			if (OwnedShip->GetShipCameraManager())
			{
				OwnedShip->GetShipCameraManager()->ZoomIn();
			}
		}
	}
	
	if (AxisValue < 0)
	{
		if (OwnedShip && OwnedShip == GetPawn())
		{
			if (OwnedShip->GetShipCameraManager())
			{
				OwnedShip->GetShipCameraManager()->ZoomOut();
			}
		}
	}
}

void ASB_PlayerController::LeftMouseButtonPressed()
{
	if (OwnedShip)
	{
		/*if (OwnedShip->GetShieldModule())
		{
			if (OwnedShip->GetShieldModule()->GetIsSetupMode() == true)
			{
				OwnedShip->GetShieldModule()->Deploy();
			}
		}
		else
		{*/
			OwnedShip->StartFireSelectedWeapons();
		//}
	}
}

void ASB_PlayerController::LeftMouseButtonReleased()
{
	if (OwnedShip)
	{
		OwnedShip->StopFireAllWeapons();
	}
}

void ASB_PlayerController::RightMouseButtonPressed()
{
	if (OwnedShip)
	{
		OwnedShip->StartAutoLockSelectedWeapon();
	}
}

void ASB_PlayerController::RightMouseButtonReleased()
{
}

void ASB_PlayerController::TabKeyPressed()
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

void ASB_PlayerController::ShiftKeyPressed()
{
	if (OwnedShip)
	{
		if (OwnedShip->GetShieldModule())
		{
			OwnedShip->GetShieldModule()->StartSetup();
		}
	}

	/*if (GetPawn() == Cast<APawn>(GetSpectatorPawn()))
	{
		//RTSCameraPawn->ToggleQuickMovement(true);
	}*/
}

void ASB_PlayerController::ShiftKeyReleased()
{
	if (OwnedShip)
	{
		if (OwnedShip->GetShieldModule())
		{
			OwnedShip->GetShieldModule()->StopSetup();
		}
	}

	/*if (GetPawn() == Cast<APawn>(GetSpectatorPawn()))
	{
		//RTSCameraPawn->ToggleQuickMovement(false);
	}*/
}

void ASB_PlayerController::SpaceBarKeyPressed()
{
	if (OwnedShip)
	{
		if (OwnedShip->GetState() == ESB_ShipState::Ready)
		{
			// If going into sniper mode without any weapon selected, select a weapon first.
			if (OwnedShip->GetShipCameraManager()->GetIsSniperMode() == false)
			{
				if (OwnedShip->GetSelectedWeaponID() == 0)
				{
					OwnedShip->SelectWeapon(1);
				}
			}

			OwnedShip->GetShipCameraManager()->ToggleSniperMode();
		}
		else
		{
			Respawn_Server();
		}
	}
}

void ASB_PlayerController::SelectWeapon1KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(0, true);
	}
}

void ASB_PlayerController::SelectWeapon2KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(1, true);
	}
}

void ASB_PlayerController::SelectWeapon3KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(2, true);
	}
}

void ASB_PlayerController::SelectWeapon4KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(3, true);
	}
}

void ASB_PlayerController::SelectWeapon5KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(4, true);
	}
}

void ASB_PlayerController::SelectAllWeaponsKeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(0, false, true);
		OwnedShip->SelectWeapon(1, false, true);
		OwnedShip->SelectWeapon(2, false, true);
		OwnedShip->SelectWeapon(3, false, true);
		OwnedShip->SelectWeapon(4, false, true);
	}
}

void ASB_PlayerController::UnselectAllWeaponsKeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(0, false, false);
		OwnedShip->SelectWeapon(1, false, false);
		OwnedShip->SelectWeapon(2, false, false);
		OwnedShip->SelectWeapon(3, false, false);
		OwnedShip->SelectWeapon(4, false, false);
	}
}

#pragma endregion