// SBGame
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "Ship/SB_TargetPoint.h"
#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ThrusterModule.h"
#include "Module/SB_ShieldModule.h"
#include "Module/Weapon/SB_WeaponModule.h"
#include "Battle/SB_BattleGameMode.h"
#include "Battle/SB_PlayerState.h"
#include "Battle/SB_HUDVehicleOTMWidget.h"
#include "SB_PlayerController.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
// Plugins
#include "RZ_UtilityLibrary.h"
// Engine
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "DestructibleActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

#pragma region +++++ Setup ...

ASB_Ship::ASB_Ship(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USB_ShipMovementComponent>(ACharacter::CharacterMovementComponentName)),
	State(ESB_ShipState::Ready)
{
	GetCapsuleComponent()->SetCapsuleSize(DEFAULTCAPSULESIZE, DEFAULTCAPSULESIZE, false);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->BodyInstance.bLockZTranslation = true;
	
	GetMesh()->DestroyComponent();

	ShipMovement = Cast<USB_ShipMovementComponent>(ACharacter::GetMovementComponent());
	//ShipCameraManager = CreateDefaultSubobject<USB_ShipCameraManager>(FName("ShipCameraManager"));
	
	//

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = nullptr;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ASB_Ship::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetComponents(ModuleSlots);

	for (const auto& ModuleSlot : ModuleSlots)
	{
		ModuleSlot->UpdateEditorMesh();
	}
}

void ASB_Ship::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	BattleGMode =  Cast<ASB_BattleGameMode>(GetWorld()->GetAuthGameMode());
	ShipData = GInstance->GetShipDataFromRow(ShipDataRowName);
	
	/*if (GInstance->GetSaveGame())
		LoadConfig(GInstance->GetSaveGame()->ShipConfig);
	else
		LoadConfig(TArray<FName>());*/

	//

	TInlineComponentArray<USB_TargetPoint*> InlineTargetPoints;
	GetComponents(InlineTargetPoints);
	TargetPoints = InlineTargetPoints;
}

void ASB_Ship::BeginPlay()
{
	Super::BeginPlay();

	// OTM widget

	bool bShouldCreateOTMWidget = false;
	
	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()) == false)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			if (PC->IsLocalController() == false)
				bShouldCreateOTMWidget = true;
		}
		else
		{
			bShouldCreateOTMWidget = true;
		}
	}

	if (bShouldCreateOTMWidget)
	{
		OTMWidget = CreateWidget<USB_HUDVehicleOTMWidget>(GetWorld(), GInstance->UISettings.HUDVehicleOTM_WBP);
		if (OTMWidget)
		{
			OTMWidget->Init(this);
		}
	}

	//

	Durability = 10000.0f; //DataManager->ShipSettings.MaxDurability;
}

void ASB_Ship::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PState = Cast<ASB_PlayerState>(NewController->PlayerState);
}


#pragma endregion

#pragma region +++++ Main ...

void ASB_Ship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSecondaryTargets();
	
	// Update circle particle
	//CircleParticleCT->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(ShipMovement->Velocity));
	
	//Debug(DeltaTime);
}

void ASB_Ship::UpdateOwnerViewData(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		UpdateOwnerViewData_Server(NewOwnerViewLocation, NewOwnerViewActor);
	
	OwnerViewLocation = NewOwnerViewLocation;
	OwnerViewActor = NewOwnerViewActor;
}

void ASB_Ship::UpdateOwnerViewData_Server_Implementation(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor)
{
	UpdateOwnerViewData(NewOwnerViewLocation, NewOwnerViewActor);
}

#pragma endregion

#pragma region +++++ Modules ...

void ASB_Ship::LoadConfig(const TArray<FName>& NewConfig, bool bSpawnEmptyModules)
{
	for (int32 Index = 0; Index != ModuleSlots.Num(); Index++)
	{
		USB_BaseModule* NewModule = nullptr;
		
		if (NewConfig.IsValidIndex(Index))
		{
			NewModule = ModuleSlots[Index]->SpawnModule(NewConfig[Index], bSpawnEmptyModules);
		}
		else
		{
			NewModule = ModuleSlots[Index]->SpawnDefaultModule(bSpawnEmptyModules);
        }


		//

		if (NewModule->GetBaseModuleData()->ModuleType == ESB_ModuleType::Weapon)
		{
			Weapons.Add(Cast<USB_WeaponModule>(NewModule));
		}
	}
}

void ASB_Ship::SaveConfig()
{
	TArray<FName> NewConfig;

	for (const auto& ModuleSlot : ModuleSlots)
	{
		NewConfig.Add(ModuleSlot->GetSpawnedModule()->GetModuleRowName());
	}

	GInstance->SaveShipConfig(NewConfig);
}

#pragma endregion

#pragma region +++++ Weapons ...

void ASB_Ship::SelectPriorityTarget(ASB_Ship* NewTargetShip)
{
}

void ASB_Ship::UpdateSecondaryTargets() // update all target, since its on tick
{
	if (BattleGMode == nullptr || PState == nullptr)
		return;
	
	TArray<AActor*> EnemyShips;
	for (auto& Ship : BattleGMode->GetSpawnedShips())
	{
		if (Ship->PState)
		{
			if (Ship->PState->GetTeam() != PState->GetTeam())
			{
				EnemyShips.Add(Ship.Get());
			}
		}
	}

	ASB_Ship* const TargetShip = Cast<ASB_Ship>(
		RZ_UtilityLibrary::GetClosestActorFromLocation(EnemyShips, GetActorLocation())
	);

	for (const auto& Weapon : Weapons)
	{
		Weapon->SetTargetShip(TargetShip);
	}
}

#pragma endregion

#pragma region +++++ Combat ...

void ASB_Ship::ApplyShipDamage(float ShipDamage, const FVector& HitLocation, AController* const InstigatorController, float FromModuleDamage)
{
	if (State == ESB_ShipState::Destroyed)
		return;

	if (Durability - ShipDamage <= 0)
	{
		Durability = 0.0f;
		UpdateState(ESB_ShipState::Destroyed);
	}
	else
	{
		Durability -= ShipDamage;
	}

	ASB_PlayerController* const InstigatorPlayerController = Cast<ASB_PlayerController>(InstigatorController);
	if (InstigatorPlayerController)
	{
		//InstigatorPlayerController->OnDamageDealt(ShipDamage, FromModuleDamage, HitLocation, ESB_PrimaryDamageType::Ship);
	}

	OnRep_Durability();
}

void ASB_Ship::UpdateState(ESB_ShipState NewState)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (NewState == ESB_ShipState::Destroyed)
	{
		//StopFireWeapons();
	}

	UpdateState_Multicast(NewState);
}

void ASB_Ship::UpdateState_Multicast_Implementation(ESB_ShipState NewState)
{
	if (NewState == ESB_ShipState::Destroyed)
	{
		GetCapsuleComponent()->SetCollisionProfileName("IgnoreAll");
		GetMesh()->SetCollisionProfileName("IgnoreAll");
		//for (const auto& BaseModule : BaseModules)
			//BaseModule->SetCollisionProfileName("IgnoreAll");
		SetActorHiddenInGame(true);

		/*AActor* DestructibleShip = GetWorld()->SpawnActorDeferred<AActor>(DataManager->GameSettings.DestructibleShipClass, GetMesh()->GetComponentTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (DestructibleShip)
		{
			UGameplayStatics::FinishSpawningActor(DestructibleShip, GetMesh()->GetComponentTransform());
		}*/

		if (OTMWidget)
		{
			OTMWidget->RemoveFromViewport();
		}

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			GInstance->ShipSettings.DestroyedParticle,
			GetActorLocation(),
			GetActorRotation(),
			FVector(GInstance->ShipSettings.DestroyedParticleScale),
			true,
			EPSCPoolMethod::None,
			true);

		OnDestroyed.Broadcast(nullptr);
	}

	State = NewState;
}

#pragma endregion

#pragma region +++++ Mesh ...

void ASB_Ship::ToggleOutline(bool bNewIsVisible, int32 StencilValue)
{
	GetMesh()->SetCustomDepthStencilValue(StencilValue);
	GetMesh()->SetRenderCustomDepth(bNewIsVisible);
}

#pragma endregion

#pragma region +++++ Replication / Debug ...

void ASB_Ship::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASB_Ship, OwnerViewLocation);
	DOREPLIFETIME(ASB_Ship, Durability);
}

void ASB_Ship::OnRep_Durability()
{
	OnDurabilityUpdated.Broadcast(Durability, ShipData->MaxDurability);
}

void ASB_Ship::Debug(float DeltaTime)
{
	FString RoleString = "None";
	FColor Color = FColor::White;

	if (GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy // ";
		Color = FColor::Orange;
	}

	FString StringToPrint;

	/*if (TargetModule)
		StringToPrint = RoleString + this->GetName() + " // TargetModule : " + TargetModule->GetName();
	else
		StringToPrint = RoleString + this->GetName() + " // TargetModule : nullptr";*/

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, StringToPrint);
}

#pragma endregion