#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "Ship/SB_ShipCameraManager.h"
#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ThrusterModule.h"
#include "Module/SB_ShieldModule.h"
#include "Module/Weapon/SB_BaseWeaponModule.h"
#include "Battle/SB_ShipOTMWidget.h"
#include "SB_PlayerController.h"
#include "SB_PlayerState.h"
#include "SB_DataManager.h"
#include "SB_GameInstance.h"
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
#include "SB_PlayerSaveGame.h"

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
	//BaseModules.Add(Cast<USB_BaseModule>(GetMesh()));
	GetMesh()->DestroyComponent();

	//CircleParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CircleParticleCT"));
	//CircleParticleCT->SetupAttachment(RootComponent);

	//

	/*CommandModule = CreateDefaultSubobject<USB_BaseModule>(TEXT("CommandModule"));
	CommandModule->SetupAttachment(GetMesh());
	ThrusterModule_Back = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Back"));
	ThrusterModule_Back->SetupAttachment(GetMesh());
	ThrusterModule_Front = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Front"));
	ThrusterModule_Front->SetupAttachment(GetMesh());
	ThrusterModule_Left = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Left"));
	ThrusterModule_Left->SetupAttachment(GetMesh());
	ThrusterModule_Right = CreateDefaultSubobject<USB_ThrusterModule>(TEXT("ThrusterModule_Right"));
	ThrusterModule_Right->SetupAttachment(GetMesh());
	ShieldModule = CreateDefaultSubobject<USB_ShieldModule>(TEXT("ShieldModule"));
	ShieldModule->SetupAttachment(GetMesh());*/

	ShipCameraManager = CreateDefaultSubobject<USB_ShipCameraManager>(FName("ShipCameraManager"));
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
	HoveredModule = nullptr;
	SelectedModule = nullptr;
}

void ASB_Ship::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetComponents(ModuleSlots);

	for (auto& ModuleSlot : ModuleSlots)
	{
		ModuleSlot->UpdateDemoMesh();
	}
}

void ASB_Ship::PreInitializeComponents()
{
	if (GetWorld()->IsGameWorld() == true)
	{	
		GInstance = Cast<USB_GameInstance>(GetGameInstance());
		
		for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager) // get from ginstance ?
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

	if (GetWorld()->IsGameWorld() == true)
	{
		GetComponents(ModuleSlots);
		GetComponents(BaseModules);
		GetComponents(ThrusterModules);
		GetComponents(WeaponModules);
	}
}

void ASB_Ship::BeginPlay()
{
	Super::BeginPlay();

	for (auto& ModuleSlot : ModuleSlots)
	{
		ModuleSlot->SpawnModule();
	}

	//SortedModules.Add(Cast<USB_BaseModule>(GetMesh()));
		
	//LoadConfig(GInstance->GetSaveGame()->ModulesRowNames);

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

	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()) == false)
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

	//

	Durability = DataManager->ShipSettings.MaxDurability;
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

	HoverModule();
	
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

#pragma endregion

#pragma region +++++ Modules ...

void ASB_Ship::LoadModules(const TArray<FName>& NewConfig)
{
	const TInlineComponentArray<USB_BaseModule*> TmpBaseModules;

	for (auto& ModuleSlot : ModuleSlots)
	{
		const FSB_BaseModuleData& BaseModuleData = DataManager->GetBaseModuleDataFromRowRef(ModuleSlot->GetModuleSlotData().DefaultModuleRowName);

		
	}
	
	/* 
	// Destroy old modules
	TInlineComponentArray<class USB_BaseModule*> InlineBaseModules;
	for (auto& BaseModule : InlineBaseModules)
	if (BaseModule->GetModuleSlotData()->bShouldUpdateModule == true)
	{
	BaseModule->DestroyComponent();
	}

	Create new modules
	uint8 Index = 0;
	const TArray<FName> DefaultConfigRowNames = DataManager->GetCarrierShipConfigDT()->GetRowNames();
	for (auto& DefaultConfigRowName : DefaultConfigRowNames)
	{
	const FSB_ModuleSlotData* const SlotData = DataManager->GetCarrierModuleSlotFromRow(DefaultConfigRowName);
	if (SlotData->bShouldUpdateModule == false)
	break;
		
	if (NewConfig.IsValidIndex(Index)) // Valid config RowName
	{
	const FSB_BaseModuleData* const ModuleData = DataManager->GetBaseModuleDataFromRow(NewConfig[Index]);
	if (ModuleData)
	{
	SpawnModule(SlotData, NewConfig[Index]);
	}
	else
	{
	SpawnModule(SlotData, SlotData->DefaultModuleRowName);
	}
	}
	else // Spawn default
	{
	SpawnModule(SlotData, SlotData->DefaultModuleRowName);
	}

	Index++;
	}*/
}

/*void ASB_Ship::SpawnModule(const FSB_ModuleSlotData* const ModuleSlotData, const FName& RowName)
{
	const FSB_BaseModuleData* const BaseModuleData = DataManager->GetBaseModuleDataFromRow(RowName);
	if (BaseModuleData)
	{
		const FString ComponentName = ModuleSlotData->DisplayName + "_" + FString::FromInt(ModuleSlotData->UniqueID);
		USB_BaseModule* NewModule;
		
		switch (BaseModuleData->ModuleType)
		{
			case ESB_ModuleType::PrimaryWeapon:
				NewModule = NewObject<USB_BaseWeaponModule>(this, *ComponentName);
				break;
			case ESB_ModuleType::AuxiliaryWeapon:
				NewModule = NewObject<USB_BaseWeaponModule>(this, *ComponentName);
				break;
			case ESB_ModuleType::Shield:
				NewModule = NewObject<USB_ShieldModule>(this, *ComponentName);
				break;
			default:
				NewModule = NewObject<USB_BaseModule>(this, *ComponentName);
		}

		NewModule->Init(DataManager, ModuleSlotData, RowName);
		NewModule->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NewModule->SetRelativeLocation(ModuleSlotData->RelativeLocation);
		NewModule->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASB_Ship::SpawnModule // Invalid DT row name : %s"), *RowName.ToString());
	}
}*/

void ASB_Ship::HoverModule()
{
	if (SelectedModule.IsValid() == true)
		return;
	
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		const TWeakObjectPtr<USB_BaseModule> NewHoveredModule = Cast<USB_BaseModule>(HitResult.Component);
	
		if (NewHoveredModule != HoveredModule)
		{
			if (HoveredModule.IsValid())
			{
				HoveredModule->OnHoverStop();
			}

			if (NewHoveredModule.IsValid())
			{
				NewHoveredModule->OnHoverStart();
			}

			HoveredModule = NewHoveredModule;
			OnModuleHovered.Broadcast(HoveredModule.Get());
		}
	}
}

void ASB_Ship::SelectModuleByID(uint8 ModuleID)
{
	if (SelectedModule.IsValid())
	{
		SelectedModule->OnUnselect();
	}

	if (BaseModules[ModuleID]->GetBaseModuleData()->bIsSelectable)
	{
		SelectedModule = BaseModules[ModuleID];
		SelectedModule->OnSelect();
	}

	OnModuleSelected.Broadcast(SelectedModule.Get());
}

void ASB_Ship::SelectModule(USB_BaseModule* const ModuleToSelect)
{
	if (SelectedModule == ModuleToSelect)
		return;

	if (SelectedModule.IsValid())
	{
		SelectedModule->OnUnselect();
	}

	SelectedModule = ModuleToSelect;
	if (ModuleToSelect)
	{
		if (ModuleToSelect->GetBaseModuleData()->bIsSelectable)
		{
			SelectedModule = ModuleToSelect;

			if (SelectedModule.IsValid())
				SelectedModule->OnSelect();
		}
		else
		{
			SelectedModule = nullptr;
		}
	}
		
	OnModuleSelected.Broadcast(SelectedModule.Get());
}

void ASB_Ship::ReplaceModule(uint8 ModuleID, const FName& ModuleDataRowName)
{
	/*if (BaseModules.IsValidIndex(ModuleID) == false)
		return;

	// Unhover + Unselect all modules.

	for (auto& Module : BaseModules)
	{
		Module->OnHoverStop();
		Module->OnUnselect();
	}

	//

	const ESB_ModuleSlotType SlotTypeTMP = BaseModules[ModuleID]->SlotType;
	const FVector RelativeLocationTMP = BaseModules[ModuleID]->GetRelativeLocation();
	
	BaseModules[ModuleID]->DestroyComponent();

	if (DataManager->GetBaseModuleDataFromRow(ModuleDataRowName)->bIsSelectable)
	{
		FString WeaponName = "WeaponModule_" + FString::FromInt(ModuleID);
		USB_BaseWeaponModule* const NewWeaponModule = NewObject<USB_BaseWeaponModule>(this, *WeaponName);
		if (NewWeaponModule)
		{
			UE_LOG(LogTemp, Warning, TEXT("ASB_Ship::ReplaceModule // DataRowName : %s // WeaponName : %s"), *ModuleDataRowName.ToString(), *WeaponName);

			NewWeaponModule->SlotType = SlotTypeTMP;
			NewWeaponModule->ModuleName = ModuleDataRowName;
			NewWeaponModule->ModuleID = ModuleID;
			NewWeaponModule->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			NewWeaponModule->SetRelativeLocation(RelativeLocationTMP);
			NewWeaponModule->RegisterComponent();
			BaseModules[ModuleID] = NewWeaponModule;
			SelectModule(NewWeaponModule);

			OnModuleReplaced.Broadcast();

			//NewWeaponModule->InitData(ModuleDataRowName);
			//NewWeaponModule->SetWorldScale3D(FVector(1000.0f));
		}
	}*/

	//SelectModule(nullptr);
	//SaveConfig();
}
void ASB_Ship::SaveConfig()
{
	if (GInstance)
	{
		GetComponents(BaseModules);

		TArray<FName> NewConfig;
		
		for (auto& Module : BaseModules)
		{
			//NewConfig.Add(Module->ModuleName);
			
		}

		GInstance->SetNewShipConfig(NewConfig);
		
		UE_LOG(LogTemp, Warning, TEXT("ASB_Ship::SaveConfig // Config saved."));
	}
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

	DOREPLIFETIME(ASB_Ship, OwnerViewRotation);
	DOREPLIFETIME(ASB_Ship, OwnerViewLocation);
	DOREPLIFETIME(ASB_Ship, Durability);
}

void ASB_Ship::OnRep_Durability()
{
	OnDurabilityUpdated.Broadcast(Durability, DataManager->ShipSettings.MaxDurability);
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