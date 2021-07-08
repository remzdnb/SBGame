#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_ShipPowerComponent.h"
#include "SB_ShipCombatComponent.h"
#include "SB_ModuleSlot.h"
#include "SB_PowerModule.h"
#include "SB_ThrusterModule.h"
#include "SB_WeaponModule.h"
#include "SB_ShieldModule.h"
#include "SB_DataManager.h"
#include "SB_ShipCameraComponent.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

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

	GetMesh()->SetRelativeLocation(DEFAULTRELATIVEMESHLOCATION);
	GetMesh()->SetRelativeRotation(DEFAULTRELATIVEMESHROTATION);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetMesh()->bEnablePerPolyCollision = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetMesh()->SetCustomDepthStencilValue(1);

	CameraArmCT = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArmCT"));
	CameraArmCT->SetupAttachment(RootComponent);
	CameraArmCT->bUsePawnControlRotation = false;
	CameraArmCT->bDoCollisionTest = true;

	CameraCT = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCT"));
	CameraCT->SetupAttachment(CameraArmCT, USpringArmComponent::SocketName);
	CameraCT->bUsePawnControlRotation = false;

	CircleParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CircleParticleCT"));
	CircleParticleCT->SetupAttachment(RootComponent);

	//

	LeftThrusterSlotCT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("LeftThrusterSlotCT"));
	LeftThrusterSlotCT->SetupAttachment(GetMesh());
	BackThrusterSlotCT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("BackThrusterSlotCT"));
	BackThrusterSlotCT->SetupAttachment(GetMesh());
	RightThrusterSlotCT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("RightThrusterSlotCT"));
	RightThrusterSlotCT->SetupAttachment(GetMesh());
	PowerSlot_1_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("PowerSlot_1_CT"));
	PowerSlot_1_CT->SetupAttachment(GetMesh());
	PowerSlot_2_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("PowerSlot_2_CT"));
	PowerSlot_2_CT->SetupAttachment(GetMesh());
	PowerSlot_3_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("PowerSlot_3_CT"));
	PowerSlot_3_CT->SetupAttachment(GetMesh());
	PowerSlot_4_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("PowerSlot_4_CT"));
	PowerSlot_4_CT->SetupAttachment(GetMesh());
	WeaponSlot_1_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("WeaponSlot_1_CT"));
	WeaponSlot_1_CT->SetupAttachment(GetMesh());
	WeaponSlot_2_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("WeaponSlot_2_CT"));
	WeaponSlot_2_CT->SetupAttachment(GetMesh());
	WeaponSlot_3_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("WeaponSlot_3_CT"));
	WeaponSlot_3_CT->SetupAttachment(GetMesh());
	WeaponSlot_4_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("WeaponSlot_4_CT"));
	WeaponSlot_4_CT->SetupAttachment(GetMesh());
	WeaponSlot_5_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("WeaponSlot_5_CT"));
	WeaponSlot_5_CT->SetupAttachment(GetMesh());
	ShieldSlot_CT = CreateDefaultSubobject<USB_ModuleSlot>(TEXT("ShieldSlot_CT"));
	ShieldSlot_CT->SetupAttachment(GetMesh());

	//

	ShipCameraCT = CreateDefaultSubobject<USB_ShipCameraComponent>(FName("ShipCameraCT"));
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
}

void ASB_Ship::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (GetWorld()->IsGameWorld() == false)
		return;

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}
}

void ASB_Ship::BeginPlay()
{
	Super::BeginPlay();

	// Spawn and attach modules.
	uint8 Index = 0;
	TInlineComponentArray<USB_ModuleSlot*> ComponentArray;
	GetComponents(ComponentArray);
	for (auto& Component : ComponentArray)
	{
		USB_ModuleSlot* ModuleSlot = Cast<USB_ModuleSlot>(Component);
		if (ModuleSlot)
		{
			if (ModuleSlot->GetModuleName() == "")
				return;

			const FSB_BaseModuleData* const ModuleData = DataManager->GetBaseModuleDataFromRow(ModuleSlot->GetModuleName());
			if (ModuleData)
			{
				if (ModuleData->ModuleType == ESB_ModuleType::Power)
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
				else if (ModuleData->ModuleType == ESB_ModuleType::Thruster)
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
				else if (ModuleData->ModuleType == ESB_ModuleType::Weapon)
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
			}
		}

		Index++;
	}
}

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

void ASB_Ship::UpdateOwnerViewData(const FRotator& NewOwnerViewRotation, const FVector& NewOwnerViewLocation)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		UpdateOwnerViewData_Server(NewOwnerViewRotation, NewOwnerViewLocation);

	OwnerViewRotation = NewOwnerViewRotation;
	OwnerViewLocation = NewOwnerViewLocation;
}

void ASB_Ship::UpdateOwnerViewData_Server_Implementation(const FRotator& NewOwnerViewRotation, const FVector& NewOwnerViewLocation)
{
	UpdateOwnerViewData(NewOwnerViewRotation, NewOwnerViewLocation);
}

void ASB_Ship::SelectWeapon(uint8 WeaponID)
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		SelectWeapon_Server(WeaponID);

	if (WeaponModules.IsValidIndex(WeaponID))
	{
		if (WeaponModules[WeaponID])
		{
			WeaponModules[WeaponID]->ToggleSelection();
		}
	}
}

void ASB_Ship::SelectWeapon_Server_Implementation(uint8 WeaponID)
{
	SelectWeapon(WeaponID);
}

void ASB_Ship::StartFireWeapons()
{
	if (State != ESB_ShipState::Ready)
		return;

	if (GetLocalRole() < ROLE_Authority)
		StartFireWeapons_Server();

	for (auto& WeaponModule : WeaponModules)
	{
		WeaponModule->SetWantsToFire(true);
	}
}

void ASB_Ship::StartFireWeapons_Server_Implementation()
{
	StartFireWeapons();
}

void ASB_Ship::StopFireWeapons()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		StopFireWeapons_Server();
	}

	for (auto& WeaponModule : WeaponModules)
	{
		WeaponModule->SetWantsToFire(false);
	}
}

void ASB_Ship::StopFireWeapons_Server_Implementation()
{
	StopFireWeapons();
}

void ASB_Ship::UpdateState(ESB_ShipState NewState)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (NewState == ESB_ShipState::Destroyed)
	{
		StopFireWeapons();
	}

	UpdateState_Multicast(NewState);
}

void ASB_Ship::UpdateState_Multicast_Implementation(ESB_ShipState NewState)
{
	if (NewState == ESB_ShipState::Destroyed)
	{
		GetMesh()->SetMaterial(0, DataManager->ShipSettings.DestroyedMaterial);

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