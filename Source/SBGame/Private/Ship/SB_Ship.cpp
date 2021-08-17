#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ThrusterModule.h"
#include "Module/SB_ShieldModule.h"
#include "Module/Weapon/SB_BaseWeaponModule.h"
#include "Battle/SB_ShipOTMWidget.h"
#include "SB_PlayerController.h"
#include "SB_PlayerState.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
#include "SB_UtilityLibrary.h"
//
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

	//CircleParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CircleParticleCT"));
	//CircleParticleCT->SetupAttachment(RootComponent);

	//ShipCameraManager = CreateDefaultSubobject<USB_ShipCameraManager>(FName("ShipCameraManager"));
	ShipMovement = Cast<USB_ShipMovementComponent>(ACharacter::GetMovementComponent());
	
	/*for (uint8 Index = 0; Index < MAXAUTOLOCKCOMPONENTS; Index++)
	{
		USceneComponent* const NewAutoLockCT = CreateDefaultSubobject<USceneComponent>(FName("AutoLockCT_0" + FString::FromInt(Index)));
		NewAutoLockCT->SetupAttachment(RootComponent);
		AutoLockCTs.Add(NewAutoLockCT);
	}*/

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

	//

	SelectedWeaponID = 0;
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
	ShipData = GInstance->GetShipDataFromRow(ShipDataRowName);

	if (GInstance->GetSaveGame())
		LoadConfig(GInstance->GetSaveGame()->ShipConfig);
	else
		LoadConfig(TArray<FName>());

	for (const auto& ModuleSlot : ModuleSlots)
	{
		if (ModuleSlot->GetModuleSlotData().Type == ESB_ModuleType::PrimaryWeapon &&
			ModuleSlot->GetSpawnedModule())
		{
			PrimaryWeapons.Add(Cast<USB_BaseWeaponModule>(ModuleSlot->GetSpawnedModule()));
		}

		if (ModuleSlot->GetModuleSlotData().Type == ESB_ModuleType::AuxiliaryWeapon &&
			ModuleSlot->GetSpawnedModule())
		{
			AuxiliaryWeapons.Add(Cast<USB_BaseWeaponModule>(ModuleSlot->GetSpawnedModule()));
		}
	}
}

void ASB_Ship::BeginPlay()
{
	Super::BeginPlay();
	
	// Create sniper cameras

	/*uint8 Index = 0;
	for (auto& WeaponModule : WeaponModules)
	{
		const FName CameraName = *("WeaponCamera_" + FString::FromInt(Index));
		UCameraComponent* const NewWeaponCamera = NewObject<UCameraComponent>(this, CameraName);
		if (NewWeaponCamera)
		{
			NewWeaponCamera->SetupAttachment(GetMesh());
			NewWeaponCamera->SetRelativeLocation(WeaponModule->GetRelativeLocation());
			NewWeaponCamera->RegisterComponent();
			ShipCameraManager->AddSniperCamera(NewWeaponCamera);
		}

		Index++;
	}*/

	// Create OTM widget

	/*if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()) == false)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			if (PC->IsLocalController())
			{
				//LoadModules();
			}
			else
			{
				OTMWidget = CreateWidget<USB_ShipOTMWidget>(GetWorld(), GInstance->UISettings.ShipOTM_WBP);
				if (OTMWidget)
				{
					OTMWidget->Init(DataManager, this);
				}
			}
		}
		else
		{
			OTMWidget = CreateWidget<USB_ShipOTMWidget>(GetWorld(), GInstance->UISettings.ShipOTM_WBP);
			if (OTMWidget)
			{
				OTMWidget->Init(DataManager, this);
			}
		}
	}*/

	//

	Durability = 10000.0f; //DataManager->ShipSettings.MaxDurability;
}

void ASB_Ship::OnRep_PlayerState()
{
	PState = Cast<ASB_PlayerState>(GetPlayerState());
	if (PState)
	{
		//LoadConfig(PState->GetPlayerSaveGame()->ShipConfig);
	}
}

#pragma endregion

#pragma region +++++ Main ...

void ASB_Ship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*for (auto& WeaponModule : WeaponModules)
	{
		WeaponModule->TargetModule = TargetModule;
	}*/

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

void ASB_Ship::LoadConfig(const TArray<FName>& NewConfig)
{
	for (int32 Index = 0; Index != ModuleSlots.Num(); Index++)
	{
		if (NewConfig.IsValidIndex(Index))
		{
			ModuleSlots[Index]->SpawnModule(NewConfig[Index]);
		}
		else
		{
			ModuleSlots[Index]->SpawnDefaultModule();
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

void ASB_Ship::SelectWeapon(uint8 WeaponID, bool bToggleSelection, bool bNewIsSelected)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		SelectWeapon_Server(WeaponID, bToggleSelection, bNewIsSelected);

	if (PrimaryWeapons.IsValidIndex(WeaponID))
	{
		if (PrimaryWeapons[WeaponID])
		{
			//WeaponModules[WeaponID]->SetIsSelected(bToggleSelection, bNewIsSelected);
		}
	}

	/*if (bToggleSelection == false)
	{
		for (auto& WeaponModule : WeaponModules)
		{
			WeaponModule->ToggleSelection(false);
		}
	}

	if (WeaponModules.IsValidIndex(WeaponID - 1))
	{
		if (WeaponModules[WeaponID - 1])
		{
			WeaponModules[WeaponID - 1]->ToggleSelection(true);
		}

		SelectedWeaponID = WeaponID;

		if (ShipCameraManager)
		{
			ShipCameraManager->SetActiveSniperCamera(WeaponID - 1);
		}

		SelectedWeaponUpdatedEvent.Broadcast(SelectedWeaponID);
	}*/
}

void ASB_Ship::SelectWeapon_Server_Implementation(uint8 WeaponID, bool bToggleSelection, bool bNewIsSelected)
{
	SelectWeapon(WeaponID, bToggleSelection, bNewIsSelected);
}

void ASB_Ship::StartFireSelectedWeapons()
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		StartFireSelectedWeapons_Server();

	for (auto& WeaponModule : PrimaryWeapons)
	{
		if (WeaponModule->GetIsSelected())
		{
			WeaponModule->SetWantsToFire(true);
		}
	}
}

void ASB_Ship::StartFireSelectedWeapons_Server_Implementation()
{
	StartFireSelectedWeapons();
}

void ASB_Ship::StopFireAllWeapons()
{
	if (GetLocalRole() < ROLE_Authority)
		StopFireAllWeapons_Server();

	for (auto& WeaponModule : PrimaryWeapons)
	{
		WeaponModule->SetWantsToFire(false);
	}
}

void ASB_Ship::StopFireAllWeapons_Server_Implementation()
{
	StopFireAllWeapons();
}

void ASB_Ship::StartAutoLockSelectedWeapon()
{
	/*if (WeaponModules.IsValidIndex(SelectedWeaponID - 1))
	{
		WeaponModules[SelectedWeaponID - 1]->SetTargetShip(Cast<ASB_Ship>(OwnerViewActor));
	}*/
}

void ASB_Ship::StopAutoLockSelectedWeapon()
{
	/*if (WeaponModules.IsValidIndex(SelectedWeaponID - 1))
	{
		WeaponModules[SelectedWeaponID - 1]->SetTargetShip(nullptr);
	}*/
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
		InstigatorPlayerController->OnDamageDealt(ShipDamage, FromModuleDamage, HitLocation, ESB_PrimaryDamageType::Ship);
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