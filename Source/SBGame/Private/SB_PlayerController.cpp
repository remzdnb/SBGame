#include "SB_PlayerController.h"
#include "SB_PlayerState.h"
#include "SB_GameInstance.h"
#include "SB_GameMode.h"
#include "SB_GameState.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "Ship/SB_ShipCameraManager.h"
#include "Battle/SB_ShipOTMWidget.h"
#include "Module/SB_ShieldModule.h"
#include "Battle/SB_SpectatorPawn.h"
#include "RZ_UIManager.h"
#include "Battle/SB_BattleMenuWidget.h"
#include "Battle/SB_BattleHUDWidget.h"
#include "SB_CursorWidget.h"
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

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	GMode = Cast<ASB_GameMode>(GetWorld()->GetAuthGameMode());
	PState = Cast<ASB_PlayerState>(PlayerState);
}

void ASB_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UIManager = Cast<ARZ_UIManager>(GetHUD());
	
	if (IsLocalController())
	{
		CursorWidget = CreateWidget<USB_CursorWidget>(GetWorld(), DataManager->UISettings.Cursor_WBP);
		if (CursorWidget)
		{
			SetMouseCursorWidget(EMouseCursor::Default, Cast<UUserWidget>(CursorWidget));
		}
		
		// Battle player start.
		if (DataManager->GameSettings.GameType == ESB_GameType::Battle)
		{
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

			/*if (UIManager)
			{
				UIManager->ToggleHUD(true);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
				bShowMouseCursor = false;
			}*/
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

	//SetControlRotation(FRotator(0.0f, OwnedShip->GetShipMovement()->GetTargetRotationYaw(), 0.0f));
}

void ASB_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnedShip)
	{
		if (OwnedShip->GetShipMovement())
		{
			float LerpedYaw = FMath::Lerp(GetControlRotation().Yaw, OwnedShip->GetShipMovement()->GetTargetRotationYaw(), DataManager->ShipSettings.TurnInertia);
			//SetControlRotation(FRotator(0.0f, LerpedYaw, 0.0f));
		}
	}
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

void ASB_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OwnedShip = Cast<ASB_Ship>(GetPawn());
	if (OwnedShip)
	{
		OwnedShip->OnDestroyed.AddUniqueDynamic(this, &ASB_PlayerController::OnOwnedShipDestroyed);
		OnNewOwnedShip.Broadcast(OwnedShip);
		SetControlRotation(FRotator(-45.0f, OwnedShip->GetActorRotation().Yaw, 0.0f));
	}
}

void ASB_PlayerController::OnOwnedShipDestroyed(const APlayerState* const InstigatorPS)
{
	
}

void ASB_PlayerController::OnDamageDealt(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType)
{
	OnDamageDealt_Client(PrimaryDamage, SecondaryDamage, HitLocation, PrimaryDamageType);
}

void ASB_PlayerController::OnDamageDealt_Client_Implementation(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType)
{
	FLinearColor NewColor = FLinearColor::White;
	if (PrimaryDamageType == ESB_PrimaryDamageType::Ship)
		NewColor = FLinearColor::Red;
	if (PrimaryDamageType == ESB_PrimaryDamageType::Shield)
		NewColor = FLinearColor::Blue;
	
	URZ_DamageMarkerWidget* const NewDamageMarker = CreateWidget<URZ_DamageMarkerWidget>(GetWorld(), DataManager->UISettings.DamageMarker_WBP);
	if (NewDamageMarker)
	{
		NewDamageMarker->Init(PrimaryDamage, SecondaryDamage, HitLocation, NewColor);
		UIManager->AddHUDWidget(NewDamageMarker);
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

	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ASB_PlayerController::RightMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ASB_PlayerController::RightMouseButtonReleased).bConsumeInput = false;
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
	if (DataManager == nullptr)
		return;

	if (OwnedShip && OwnedShip == GetPawn() && DataManager->GameSettings.GameType == ESB_GameType::Battle)
	{
		if (OwnedShip->GetShipMovement())
		{
			OwnedShip->GetShipMovement()->MoveForward(AxisValue);
		}
	}
}

void ASB_PlayerController::MoveRightAxis(float AxisValue)
{
	if (DataManager == nullptr)
		return;

	if (OwnedShip && OwnedShip == GetPawn() && DataManager->GameSettings.GameType == ESB_GameType::Battle)
	{
		if (OwnedShip->GetShipMovement())
		{
			OwnedShip->GetShipMovement()->TurnRight(AxisValue);
		}
	}
}


void ASB_PlayerController::MouseWheelAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraManager())
		{
			if (AxisValue > 0)
			{
				OwnedShip->GetShipCameraManager()->Zoom(true);
			}

			if (AxisValue < 0)
			{
				OwnedShip->GetShipCameraManager()->Zoom(false);
			}
		}
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

void ASB_PlayerController::ShiftKeyPressed()
{
	if (OwnedShip)
	{
		if (OwnedShip->ShieldModule)
		{
			if (OwnedShip->ShieldModule->GetShieldState() == ESB_ShieldState::Ready)
			{
				OwnedShip->ShieldModule->StartSetup();
			}
			else
			{
				OwnedShip->ShieldModule->Undeploy();
			}
		}
	}
}

void ASB_PlayerController::ShiftKeyReleased()
{
	if (OwnedShip)
	{
		if (OwnedShip->ShieldModule)
		{
			OwnedShip->ShieldModule->StopSetup();
		}
	}
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