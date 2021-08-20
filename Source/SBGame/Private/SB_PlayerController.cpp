#include "SB_PlayerController.h"
#include "SB_GameInstance.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "SB_PlayerSaveGame.h"
// UtilityPlugin
#include "RZ_CameraActor.h"
// UIPlugin
#include "RZ_UIManager.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ASB_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
}

void ASB_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UIManager = Cast<ARZ_UIManager>(GetHUD());
	
	if (IsLocalController())
	{
		CameraActor = GetWorld()->SpawnActorDeferred<ARZ_CameraActor>(
			ARZ_CameraActor::StaticClass(),
			FTransform(),
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
		if (CameraActor)
		{
			UGameplayStatics::FinishSpawningActor(CameraActor, FTransform());
			SetViewTargetWithBlend(CameraActor, 0.0f);
		}
		
		/*CursorWidget = CreateWidget<USB_CursorWidget>(GetWorld(), DataManager->UISettings.Cursor_WBP);
		if (CursorWidget)
		{
			SetMouseCursorWidget(EMouseCursor::Default, Cast<UUserWidget>(CursorWidget));
		}*/
	}
}

void ASB_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnedShip)
	{
		if (OwnedShip->GetShipMovement())
		{
			//float LerpedYaw = FMath::Lerp(GetControlRotation().Yaw, OwnedShip->GetShipMovement()->GetTargetRotationYaw(), DataManager->ShipSettings.TurnInertia);
			//SetControlRotation(FRotator(0.0f, LerpedYaw, 0.0f));
		}
	}
}

void ASB_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OwnedShip = Cast<ASB_Ship>(GetPawn());
	if (OwnedShip)
	{
		//OwnedShip->OnDestroyed.AddUniqueDynamic(this, &ASB_PlayerController::OnOwnedShipDestroyed);
		OnNewOwnedShip.Broadcast(OwnedShip);
		//SetControlRotation(FRotator(-45.0f, OwnedShip->GetActorRotation().Yaw, 0.0f));
	}
}

#pragma region +++++ Input ...

void ASB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis("LookUp", this, &ASB_PlayerController::LookUpAxis).bConsumeInput = false;
	InputComponent->BindAxis("LookRight", this, &ASB_PlayerController::LookRightAxis).bConsumeInput = false;
	InputComponent->BindAxis("MouseWheel", this, &ASB_PlayerController::ZoomAxis).bConsumeInput = false;
	
	InputComponent->BindAxis("MoveForward", this, &ASB_PlayerController::MoveForwardAxis).bConsumeInput = false;
	InputComponent->BindAxis("MoveRight", this, &ASB_PlayerController::MoveRightAxis).bConsumeInput = false;
	
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
	if (CameraActor)
	{
		CameraActor->AddPitchInput(AxisValue);	
	}
}

void ASB_PlayerController::LookRightAxis(float AxisValue)
{
	if (CameraActor)
	{
		CameraActor->AddYawInput(AxisValue);
	}
}

void ASB_PlayerController::ZoomAxis(float AxisValue)
{
	if (CameraActor)
	{
		if (AxisValue > 0)
		{
			CameraActor->AddZoomInput(true);	
		}

		if (AxisValue < 0)
		{
			CameraActor->AddZoomInput(false);
		}
	}
}

void ASB_PlayerController::MoveForwardAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipMovement())
		{
			OwnedShip->GetShipMovement()->MoveForward(AxisValue);
		}
	}
}

void ASB_PlayerController::MoveRightAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipMovement())
		{
			OwnedShip->GetShipMovement()->TurnRight(AxisValue);
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
	/*if (OwnedShip)
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
	}*/
}

void ASB_PlayerController::ShiftKeyReleased()
{
	/*if (OwnedShip)
	{
		if (OwnedShip->ShieldModule)
		{
			OwnedShip->ShieldModule->StopSetup();
		}
	}*/
}

void ASB_PlayerController::SpaceBarKeyPressed()
{
	/*if (OwnedShip)
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
	}*/
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