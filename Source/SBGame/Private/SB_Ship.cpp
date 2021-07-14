#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_ShipPowerComponent.h"
#include "SB_ShipCombatComponent.h"
#include "SB_ShipOTMWidget.h"
#include "SB_PowerModule.h"
#include "SB_ThrusterModule.h"
#include "SB_WeaponModule.h"
#include "SB_ShieldModule.h"
#include "SB_GameInstance.h"
#include "SB_DataManager.h"
#include "SB_ShipCameraManager.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "DestructibleActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

#pragma region +++++ Setup ...

ASB_Ship::ASB_Ship(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USB_ShipMovementComponent>(ACharacter::CharacterMovementComponentName).SetDefaultSubobjectClass<USB_BaseModule>(ACharacter::MeshComponentName)),
	State(ESB_ShipState::Ready)
{
	GetCapsuleComponent()->SetCapsuleSize(DEFAULTCAPSULESIZE, DEFAULTCAPSULESIZE, false);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->BodyInstance.bLockZTranslation = true;

	GetMesh()->SetRelativeLocation(DEFAULTRELATIVEMESHLOCATION);
	GetMesh()->SetRelativeRotation(DEFAULTRELATIVEMESHROTATION);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetMesh()->bEnablePerPolyCollision = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	CircleParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CircleParticleCT"));
	CircleParticleCT->SetupAttachment(RootComponent);

	//

	CommandModule = CreateDefaultSubobject<USB_BaseModule>(TEXT("CommandModule"));
	CommandModule->SetupAttachment(GetMesh());
	ThrusterModule_Back = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Back"));
	ThrusterModule_Back->SetupAttachment(GetMesh());
	ThrusterModules.Add(ThrusterModule_Back);
	ThrusterModule_Front = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Front"));
	ThrusterModule_Front->SetupAttachment(GetMesh());
	ThrusterModules.Add(ThrusterModule_Front);
	ThrusterModule_Left = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Left"));
	ThrusterModule_Left->SetupAttachment(GetMesh());
	ThrusterModules.Add(ThrusterModule_Left);
	ThrusterModule_Right = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Right"));
	ThrusterModule_Right->SetupAttachment(GetMesh());
	ThrusterModules.Add(ThrusterModule_Right);
	ShieldModule = CreateDefaultSubobject<USB_ShieldModule>(TEXT("ShieldModule"));
	ShieldModule->SetupAttachment(GetMesh());

	//

	for (uint8 Index = 0; Index < MAXAUTOLOCKCOMPONENTS; Index++)
	{
		USceneComponent* const NewAutoLockCT = CreateDefaultSubobject<USceneComponent>(FName("AutoLockCT_0" + FString::FromInt(Index)));
		NewAutoLockCT->SetupAttachment(RootComponent);
		AutoLockCTs.Add(NewAutoLockCT);
	}

	//

	ShipCameraManager = CreateDefaultSubobject<USB_ShipCameraManager>(FName("ShipCameraManager"));
	PowerCT = CreateDefaultSubobject<USB_ShipPowerComponent>(FName("PowerCT"));
	MovementCT = Cast<USB_ShipMovementComponent>(GetMovementComponent());
	CombatCT = CreateDefaultSubobject<USB_ShipCombatComponent>(FName("CombatCT"));

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

void ASB_Ship::PreInitializeComponents()
{
	if (GetWorld()->IsGameWorld() == true)
	{
		for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
		{
			DataManager = *NewDataManager;
			break;
		}
	}
	
	Super::PreInitializeComponents();
}

void ASB_Ship::PostInitializeComponents()
{

	
	Super::PostInitializeComponents();
	

	//DataManager = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetDataManager();


}

void ASB_Ship::BeginPlay()
{
	Super::BeginPlay();

	// Init modules

	GetComponents(WeaponModules);
	
	/*
	
	uint8 Index = 0;
	TInlineComponentArray<USB_ModuleSlot*> ComponentArray;
	GetComponents(ComponentArray);
	for (auto& Component : ComponentArray)
	{
		USB_ModuleSlot* ModuleSlot = Cast<USB_ModuleSlot>(Component);
		if (ModuleSlot)
		{
			const FSB_BaseModuleData* const ModuleData = DataManager->GetBaseModuleDataFromRow(ModuleSlot->GetModuleName());
			if (ModuleData)
			{
				if (ModuleData->ModuleType == ESB_ModuleType::Thruster)
				{
					const FName ModuleName = *("ThrusterModule_" + ModuleSlot->GetModuleName().ToString() + "_" + FString::FromInt(Index));
					USB_ThrusterModule* const NewThrusterModule = NewObject<USB_ThrusterModule>(this, ModuleName);
					if (NewThrusterModule)
					{
						NewThrusterModule->SetupAttachment(ModuleSlot);
						NewThrusterModule->RegisterComponent();
						NewThrusterModule->SetSkeletalMesh(ModuleData->SkeletalMesh);
						NewThrusterModule->Init(DataManager, ModuleSlot->GetSlotName(), ModuleSlot->GetModuleName());
						BaseModules.Add(NewThrusterModule);
						ThrusterModules.Add(NewThrusterModule);
					}
				}
				if (ModuleData->ModuleType == ESB_ModuleType::Weapon)
				{
					const FName ModuleName = *("WeaponModule_" + ModuleSlot->GetModuleName().ToString() + "_" + FString::FromInt(Index));
					USB_WeaponModule* const NewWeaponModule = NewObject<USB_WeaponModule>(this, ModuleName);
					if (NewWeaponModule)
					{
						NewWeaponModule->SetupAttachment(ModuleSlot);
						NewWeaponModule->RegisterComponent();
						NewWeaponModule->SetSkeletalMesh(ModuleData->SkeletalMesh);
						NewWeaponModule->Init(DataManager, ModuleSlot->GetSlotName(), ModuleSlot->GetModuleName());
						BaseModules.Add(NewWeaponModule);
						WeaponModules.Add(NewWeaponModule);

						const FName CameraName = *("WeaponCamera_" + FString::FromInt(Index));
						UCameraComponent* const NewWeaponCamera = NewObject<UCameraComponent>(this, CameraName);
						if (NewWeaponCamera)
						{
							NewWeaponCamera->SetupAttachment(NewWeaponModule, "ViewSocket");
							NewWeaponCamera->RegisterComponent();
							ShipCameraManager->AddSniperCamera(NewWeaponCamera);
						}
					}
				}
				else if (ModuleData->ModuleType == ESB_ModuleType::Shield)
				{
					const FName ModuleName = *("ShieldModule_" + ModuleSlot->GetModuleName().ToString() + "_" + FString::FromInt(Index));
					USB_ShieldModule* const NewShieldModule = NewObject<USB_ShieldModule>(this, ModuleName);
					if (NewShieldModule)
					{
						NewShieldModule->SetupAttachment(ModuleSlot);
						NewShieldModule->RegisterComponent();
						NewShieldModule->SetSkeletalMesh(ModuleData->SkeletalMesh);
						NewShieldModule->Init(DataManager, ModuleSlot->GetSlotName(), ModuleSlot->GetModuleName());
						BaseModules.Add(NewShieldModule);
						ShieldModule = NewShieldModule;
					}
				}
				else if (ModuleData->ModuleType == ESB_ModuleType::Power)
				{
					const FName ModuleName = *("PowerModule_" + ModuleSlot->GetModuleName().ToString() + "_" + FString::FromInt(Index));
					USB_PowerModule* const NewPowerModule = NewObject<USB_PowerModule>(this, ModuleName);
					if (NewPowerModule)
					{
						NewPowerModule->SetupAttachment(ModuleSlot);
						NewPowerModule->RegisterComponent();
						NewPowerModule->SetSkeletalMesh(ModuleData->SkeletalMesh);
						NewPowerModule->Init(DataManager, ModuleSlot->GetSlotName(), ModuleSlot->GetModuleName());
						BaseModules.Add(NewPowerModule);
						PowerModules.Add(NewPowerModule);
					}
				}
			}
		}

		Index++;
	}*/

	// Create OTM widget

	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()) == false)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			if (PC->IsLocalController())
			{

			}
			else
			{
				OTMWidget = CreateWidget<USB_ShipOTMWidget>(GetWorld(), DataManager->UISettings.ShipOTM_WBP);
				if (OTMWidget)
				{
					OTMWidget->Init(DataManager, this);
				}
			}
		}
		else
		{
			OTMWidget = CreateWidget<USB_ShipOTMWidget>(GetWorld(), DataManager->UISettings.ShipOTM_WBP);
			if (OTMWidget)
			{
				OTMWidget->Init(DataManager, this);
			}
		}
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
	CircleParticleCT->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(MovementCT->Velocity));

	Debug(DeltaTime);
}

void ASB_Ship::UpdateOwnerViewData(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		UpdateOwnerViewData_Server(NewOwnerViewLocation, NewOwnerViewActor);

	//OwnerViewRotation = NewOwnerViewRotation;
	OwnerViewLocation = NewOwnerViewLocation;
	OwnerViewActor = NewOwnerViewActor;
}

void ASB_Ship::UpdateOwnerViewData_Server_Implementation(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor)
{
	UpdateOwnerViewData(NewOwnerViewLocation, NewOwnerViewActor);
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
		for (auto& ThrusterModule : ThrusterModules)
			ThrusterModule->SetCollisionProfileName("IgnoreAll");
		for (auto& WeaponModule : WeaponModules)
			WeaponModule->SetCollisionProfileName("IgnoreAll");
		SetActorHiddenInGame(true);

		AActor* DestructibleShip = GetWorld()->SpawnActorDeferred<AActor>(DataManager->GameSettings.DestructibleShipClass, GetMesh()->GetComponentTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (DestructibleShip)
		{
			UGameplayStatics::FinishSpawningActor(DestructibleShip, GetMesh()->GetComponentTransform());
		}

		if (OTMWidget)
		{
			OTMWidget->RemoveFromViewport();
		}

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DataManager->ShipSettings.DestroyedParticle,
			GetActorLocation(),
			GetActorRotation(),
			FVector(DataManager->ShipSettings.DestroyedParticleScale),
			true,
			EPSCPoolMethod::None,
			true);

		DestroyedEvent.Broadcast(nullptr);
	}

	State = NewState;
}

#pragma endregion

#pragma region +++++ Weapons ...

void ASB_Ship::SelectWeapon(uint8 WeaponID, bool bToggleSelection, bool bNewIsSelected)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		SelectWeapon_Server(WeaponID, bToggleSelection, bNewIsSelected);

	if (WeaponModules.IsValidIndex(WeaponID))
	{
		if (WeaponModules[WeaponID])
		{
			WeaponModules[WeaponID]->SetIsSelected(bToggleSelection, bNewIsSelected);
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

	for (auto& WeaponModule : WeaponModules)
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

	for (auto& WeaponModule : WeaponModules)
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

#pragma region +++++ Mesh ...

void ASB_Ship::ToggleOutline(bool bNewIsVisible)
{
	GetMesh()->SetCustomDepthStencilValue(2);
	GetMesh()->SetRenderCustomDepth(bNewIsVisible);
}

#pragma endregion

#pragma region +++++ Replication / Debug ...

void ASB_Ship::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_Ship, OwnerViewRotation);
	DOREPLIFETIME(ASB_Ship, OwnerViewLocation);
}

void ASB_Ship::Debug(float DeltaTime)
{
	if (DataManager->GameSettings.bIsDebugEnabled_Ship == false)
		return;

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