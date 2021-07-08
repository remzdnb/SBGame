#include "SB_PlayerController.h"
#include "SB_PlayerState.h"
#include "SB_GameMode.h"
#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_ShipCameraComponent.h"
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

	//SetControlRotation(FRotator(-45.0f, 0.0f, 0.0f));
}

void ASB_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnedShip)
	{
		if (OwnedShip->GetShipMovementCT())
		{
			SetControlRotation(FRotator(0.0f, OwnedShip->GetShipMovementCT()->GetRotationYaw(), 0.0f));
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
	const FCollisionQueryParams TraceParams;
	TArray<FHitResult> Hits;

	GetWorld()->LineTraceMultiByChannel(Hits, Start, End, ECC_Visibility, TraceParams);
	for (FHitResult& Hit : Hits)
	{
		if (Hit.Actor.IsValid() && Hit.Actor != GetPawn())
		{
			if (OwnedShip)
			{
				OwnedShip->UpdateOwnerViewData(GetControlRotation(), Hit.Location);
				//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 10, FColor::Green, .1f, 0.3f);
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

	InputComponent->BindAxis("LookUpAxis", this, &ASB_PlayerController::LookUpAxis).bConsumeInput = false;
	InputComponent->BindAxis("LookRightAxis", this, &ASB_PlayerController::LookRightAxis).bConsumeInput = false;
	InputComponent->BindAxis("MoveForwardAxis", this, &ASB_PlayerController::MoveForwardAxis).bConsumeInput = false;
	InputComponent->BindAxis("MoveRightAxis", this, &ASB_PlayerController::MoveRightAxis).bConsumeInput = false;
	InputComponent->BindAxis("MouseWheelAxis", this, &ASB_PlayerController::MouseWheelAxis).bConsumeInput = false;
	InputComponent->BindAction("MoveForwardKey", IE_Pressed, this, &ASB_PlayerController::MoveForwardKey).bConsumeInput = false;
	InputComponent->BindAction("MoveBackwardKey", IE_Pressed, this, &ASB_PlayerController::MoveBackwardKey).bConsumeInput = false;
	InputComponent->BindAction("TurnRightKey", IE_Pressed, this, &ASB_PlayerController::TurnRightKey).bConsumeInput = false;
	InputComponent->BindAction("TurnLeftKey", IE_Pressed, this, &ASB_PlayerController::TurnLeftKey).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ASB_PlayerController::LeftMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ASB_PlayerController::LeftMouseButtonReleased).bConsumeInput = false;
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ASB_PlayerController::RightMouseButtonPressed).bConsumeInput = false;
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ASB_PlayerController::RightMouseButtonReleased).bConsumeInput = false;
	InputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &ASB_PlayerController::MouseWheelUp).bConsumeInput = false;
	InputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &ASB_PlayerController::MouseWheelDown).bConsumeInput = false;
	//InputComponent->BindAction("TabKey", IE_Pressed, this, &ASB_PlayerController::TabKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("SpaceBarKey", IE_Pressed, this, &ASB_PlayerController::SpaceBarKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("ShiftKey", IE_Pressed, this, &ASB_PlayerController::ShiftKeyPressed).bConsumeInput = false;
	InputComponent->BindAction("ShiftKey", IE_Released, this, &ASB_PlayerController::ShiftKeyReleased).bConsumeInput = false;
	InputComponent->BindAction("1Key", IE_Pressed, this, &ASB_PlayerController::Quickslot1KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("2Key", IE_Pressed, this, &ASB_PlayerController::Quickslot2KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("3Key", IE_Pressed, this, &ASB_PlayerController::Quickslot3KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("4Key", IE_Pressed, this, &ASB_PlayerController::Quickslot4KeyPressed).bConsumeInput = false;
	InputComponent->BindAction("5Key", IE_Pressed, this, &ASB_PlayerController::Quickslot5KeyPressed).bConsumeInput = false;
}

void ASB_PlayerController::LookUpAxis(float AxisValue)
{
	if (GetStateName() == NAME_Spectating)
	{
		GetSpectatorPawn()->LookUpAtRate(AxisValue);
	}

	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraCT())
		{
			OwnedShip->GetShipCameraCT()->AddPitchInput(AxisValue);
		}
	}
}

void ASB_PlayerController::LookRightAxis(float AxisValue)
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraCT())
		{
			OwnedShip->GetShipCameraCT()->AddYawInput(AxisValue);
		}
	}
}

void ASB_PlayerController::MoveForwardAxis(float AxisValue)
{
	/*if (GetPawn() == Cast<APawn>(GetSpectatorPawn()))
	{
		RTSCameraPawn->MoveForward(AxisValue);
	}*/
}

void ASB_PlayerController::MoveRightAxis(float AxisValue)
{
	/*if (GetPawn() == Cast<APawn>(GetSpectatorPawn()))
	{
		//RTSCameraPawn->MoveRight(AxisValue);
	}*/
}

void ASB_PlayerController::MouseWheelAxis(float AxisValue)
{
	/*if (GetPawn() == Cast<APawn>(GetSpectatorPawn()))
	{
		//RTSCameraPawn->Zoom(AxisValue);
	}*/
}

void ASB_PlayerController::MoveForwardKey()
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		OwnedShip->GetShipMovementCT()->AddForwardInput(1);
	}
}

void ASB_PlayerController::MoveBackwardKey()
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		OwnedShip->GetShipMovementCT()->AddForwardInput(-1);
	}
}

void ASB_PlayerController::TurnRightKey()
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		OwnedShip->GetShipMovementCT()->AddRightInput(1);
	}
}

void ASB_PlayerController::TurnLeftKey()
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		OwnedShip->GetShipMovementCT()->AddRightInput(-1);
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
			OwnedShip->StartFireWeapons();
		//}
	}
}

void ASB_PlayerController::LeftMouseButtonReleased()
{
	if (OwnedShip)
	{
		OwnedShip->StopFireWeapons();
	}
}

void ASB_PlayerController::RightMouseButtonPressed()
{

}

void ASB_PlayerController::RightMouseButtonReleased()
{

}

void ASB_PlayerController::MouseWheelUp()
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraCT())
		{
			OwnedShip->GetShipCameraCT()->ZoomIn();
		}
	}
}

void ASB_PlayerController::MouseWheelDown()
{
	if (OwnedShip && OwnedShip == GetPawn())
	{
		if (OwnedShip->GetShipCameraCT())
		{
			OwnedShip->GetShipCameraCT()->ZoomOut();
		}
	}
}

void ASB_PlayerController::ShiftKeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->GetShieldModule()->StartSetup();
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
		OwnedShip->GetShieldModule()->StopSetup();
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
		if (OwnedShip->GetState() == ESB_ShipState::Destroyed)
		{
			Respawn_Server();
		}
	}
}

void ASB_PlayerController::Quickslot1KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(0);
	}
}

void ASB_PlayerController::Quickslot2KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(1);
	}
}

void ASB_PlayerController::Quickslot3KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(2);
	}
}

void ASB_PlayerController::Quickslot4KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(3);
	}
}

void ASB_PlayerController::Quickslot5KeyPressed()
{
	if (OwnedShip)
	{
		OwnedShip->SelectWeapon(4);
	}
}

#pragma endregion