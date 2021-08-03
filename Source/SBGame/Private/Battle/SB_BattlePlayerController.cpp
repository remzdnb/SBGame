// SBGame
#include "Battle/SB_BattlePlayerController.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "Module/SB_ShieldModule.h"
#include "SB_DataManager.h"
// UIPlugin
#include "RZ_UIManager.h"
// Engine
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ASB_BattlePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateViewTarget(DeltaTime);
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
			OwnedShip->UpdateOwnerViewData(/*GetControlRotation(), */Hit.Location, Hit.Actor.Get());
		}

		if (DataManager->GameSettings.bIsDebugEnabled_PlayerController)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, *("ASB_PlayerController::UpdateViewTarget // Actor : " + Hit.Actor->GetName() + " // Component : " + Hit.Component->GetName()));
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.Location, 500.0f, 10, FColor::Green, DeltaTime + 0.01f, 20.0f);
		}
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

void ASB_BattlePlayerController::LeftMouseButtonPressed()
{
	if (OwnedShip)
	{
		if (OwnedShip->ShieldModule->GetIsSetupMode() == 1)
		{
			OwnedShip->ShieldModule->Deploy();
		}
		else
		{
			OwnedShip->StartFireSelectedWeapons();
		}
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

