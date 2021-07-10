#include "SB_PlayerController.h"
#include "SB_PlayerState.h"
#include "SB_GameMode.h"
#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_ShipCameraManager.h"
#include "SB_ShieldModule.h"
#include "SB_SpectatorPawn.h"
#include "RZ_UIManager.h"
#include "RZ_HUDLayoutWidget.h"
#include "RZ_MenuLayoutWidget.h"
#include "SB_DataManager.h"
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
}

void ASB_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// UI setup
		UIManager = Cast<ARZ_UIManager>(GetHUD());
		if (UIManager)
		{
			UIManager->OpenHUD();
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
			bShowMouseCursor = false;
		}

		// Spectator setup
		/*OwnedSpectatorPawn = GetWorld()->SpawnActorDeferred<ASB_SpectatorPawn>(ASB_SpectatorPawn::StaticClass(), FTransform(FVector()), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (OwnedSpectatorPawn)
		{
			UGameplayStatics::FinishSpawningActor(OwnedSpectatorPawn, FTransform(FVector()));
		}*/
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
				OwnedShip->UpdateOwnerViewData(GetControlRotation(), Hit.Location, Hit.Actor.Get());

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

void ASB_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OwnedShip = Cast<ASB_Ship>(GetPawn());
	if (OwnedShip)
	{
		OwnedShip->DestroyedEvent.AddUniqueDynamic(this, &ASB_PlayerController::OnOwnedShipDestroyed);
		NewOwnedShipEvent.Broadcast(OwnedShip);
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
			OwnedShip->StartFireSelectedWeapon();
		//}
	}
}

void ASB_PlayerController::LeftMouseButtonReleased()
{
	if (OwnedShip)
	{
		OwnedShip->StopFireSelectedWeapon();
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
		OwnedShip->SelectWeapon(1);
	}
}

void ASB_PlayerController::SelectWeapon2KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(2);
	}
}

void ASB_PlayerController::SelectWeapon3KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(3);
	}
}

void ASB_PlayerController::SelectWeapon4KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(4);
	}
}

void ASB_PlayerController::SelectWeapon5KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(5);
	}
}

void ASB_PlayerController::SelectAllWeaponsKeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(1);
		OwnedShip->SelectWeapon(2);
		OwnedShip->SelectWeapon(3);
		OwnedShip->SelectWeapon(4);
		OwnedShip->SelectWeapon(5);
	}
}

void ASB_PlayerController::UnselectAllWeaponsKeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(0);
	}
}

#pragma endregion