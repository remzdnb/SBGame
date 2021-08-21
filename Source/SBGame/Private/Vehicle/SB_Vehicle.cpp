// SBGame
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_ShipMovementComponent.h"
#include "Vehicle/SB_TargetPoint.h"
#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ThrusterModule.h"
#include "Module/SB_ShieldModule.h"
#include "Module/Weapon/SB_WeaponModule.h"
#include "Battle/SB_BattleGameMode.h"
#include "Battle/SB_BattlePlayerController.h"
#include "Battle/SB_PlayerState.h"
#include "Battle/SB_HUDVehicleOTMWidget.h"
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

ASB_Vehicle::ASB_Vehicle(const FObjectInitializer& ObjectInitializer) :
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
	//ShipCameraManager = CreateDefaultSubobject<USB_VehicleCameraManager>(FName("ShipCameraManager"));
	
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

void ASB_Vehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetComponents(ModuleSlots);

	for (const auto& ModuleSlot : ModuleSlots)
	{
		ModuleSlot->UpdateEditorMesh();
	}
}

void ASB_Vehicle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	BattleGMode =  Cast<ASB_BattleGameMode>(GetWorld()->GetAuthGameMode());
	VehicleData = GInstance->GetVehicleDataFromRow(VehicleDataRowName);
	
	Durability = VehicleData->MaxDurability;
	
	/*if (GInstance->GetSaveGame())
		LoadConfig(GInstance->GetSaveGame()->ShipConfig);
	else
		LoadConfig(TArray<FName>());*/

	//

	TInlineComponentArray<USB_TargetPoint*> InlineTargetPoints;
	GetComponents(InlineTargetPoints);
	TargetPoints = InlineTargetPoints;
}

void ASB_Vehicle::BeginPlay()
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
}

void ASB_Vehicle::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PState = Cast<ASB_PlayerState>(NewController->PlayerState);
}


#pragma endregion

#pragma region +++++ Main ...

void ASB_Vehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWeaponsTargets();
	
	// Update circle particle
	//CircleParticleCT->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(ShipMovement->Velocity));
	
	//Debug(DeltaTime);
}

void ASB_Vehicle::UpdateOwnerViewData(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		UpdateOwnerViewData_Server(NewOwnerViewLocation, NewOwnerViewActor);
	
	OwnerViewLocation = NewOwnerViewLocation;
	OwnerViewActor = NewOwnerViewActor;
}

void ASB_Vehicle::UpdateOwnerViewData_Server_Implementation(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor)
{
	UpdateOwnerViewData(NewOwnerViewLocation, NewOwnerViewActor);
}

#pragma endregion

#pragma region +++++ Modules ...

void ASB_Vehicle::LoadConfig(const TArray<FName>& NewConfig, bool bSpawnEmptyModules)
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

		ModuleArray.Add(NewModule);
		NewModule->OnModuleDamaged.AddUniqueDynamic(this, &ASB_Vehicle::OnModuleDamaged);

		if (NewModule->GetBaseModuleData()->ModuleType == ESB_ModuleType::Weapon)
		{
			Weapons.Add(Cast<USB_WeaponModule>(NewModule));
		}
	}
}

void ASB_Vehicle::SaveConfig()
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

void ASB_Vehicle::SetPriorityTarget(ASB_Vehicle* NewPriorityTarget)
{
	PriorityTarget = NewPriorityTarget;
}

void ASB_Vehicle::UpdateWeaponsTargets()
{
	if (BattleGMode == nullptr || PState == nullptr)
		return;
	
	TArray<AActor*> EnemyShips;
	for (auto& Ship : BattleGMode->GetSpawnedShips())
	{
		//if (Ship.IsValid())
		//{
			if (Ship->PState)
			{
				if (Ship->PState->GetTeam() != PState->GetTeam())
				{
					EnemyShips.Add(Ship.Get());
				}
			}
		//}
	}

	ASB_Vehicle* const TargetShip = Cast<ASB_Vehicle>(
		RZ_UtilityLibrary::GetClosestActorFromLocation(EnemyShips, GetActorLocation())
	);

	for (const auto& Weapon : Weapons)
	{
		Weapon->SetTargetShip(PriorityTarget);
	}
}

#pragma endregion

#pragma region +++++ Combat ...

void ASB_Vehicle::OnModuleDamaged(
	USB_BaseModule* const ModuleRef,
	float Damage,
	const FVector& HitLocation,
	AController* const InstigatorController)
{
	ApplyShipDamage(Damage, HitLocation, InstigatorController, 0.0f);
}

void ASB_Vehicle::ApplyShipDamage(float ShipDamage, const FVector& HitLocation, AController* const InstigatorController, float FromModuleDamage)
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

	ASB_BattlePlayerController* const InstigatorPlayerController = Cast<ASB_BattlePlayerController>(InstigatorController);
	if (InstigatorPlayerController)
	{
		InstigatorPlayerController->OnDamageDealt(ShipDamage, FromModuleDamage, HitLocation, ESB_PrimaryDamageType::Ship);
	}

	OnRep_Durability();
}

void ASB_Vehicle::UpdateState(ESB_ShipState NewState)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (NewState == ESB_ShipState::Destroyed)
	{
		//StopFireWeapons();
	}

	UpdateState_Multicast(NewState);
}

void ASB_Vehicle::UpdateState_Multicast_Implementation(ESB_ShipState NewState)
{
	if (NewState == ESB_ShipState::Destroyed)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			VehicleData->OnDestroyedParticle,
			GetActorLocation(),
			GetActorRotation(),
			FVector(VehicleData->OnDestroyedParticleScale),
			true,
			EPSCPoolMethod::None,
			true
		);

		/*AActor* DestructibleShip = GetWorld()->SpawnActorDeferred<AActor>(DataManager->GameSettings.DestructibleShipClass, GetMesh()->GetComponentTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (DestructibleShip)
		{
		UGameplayStatics::FinishSpawningActor(DestructibleShip, GetMesh()->GetComponentTransform());
		}*/

		this->Destroy();
	}

	State = NewState;
}

#pragma endregion

#pragma region +++++ Mesh ...

void ASB_Vehicle::ToggleOutline(bool bNewIsVisible, int32 StencilValue)
{
	for (const auto& Module : ModuleArray)
	{
		Module->SetCustomDepthStencilValue(3);
		Module->SetRenderCustomDepth(bNewIsVisible);
	}
}

#pragma endregion

#pragma region +++++ Replication / Debug ...

void ASB_Vehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASB_Vehicle, OwnerViewLocation);
	DOREPLIFETIME(ASB_Vehicle, Durability);
}

void ASB_Vehicle::OnRep_Durability()
{
	OnVehicleDurabilityUpdated.Broadcast(Durability, VehicleData->MaxDurability);
}

void ASB_Vehicle::Debug(float DeltaTime)
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