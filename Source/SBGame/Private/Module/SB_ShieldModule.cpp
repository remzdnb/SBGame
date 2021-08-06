#include "Module/SB_ShieldModule.h"
#include "Module/SB_Shield.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipCameraManager.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
//
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

USB_ShieldModule::USB_ShieldModule()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsSetupMode = false;
	ShieldState = ESB_ShieldState::Ready;
	LastUndeployTime = 0.0f;
}

/*void USB_ShieldModule::Init(const ASB_DataManager* const NewDataManager,
	const FSB_ModuleSlotData* const NewModuleSlotData, const FName& NewModuleRowName)
{
		// Arm used to attach both setup & shield meshes.
	ShieldArm = NewObject<USpringArmComponent>(GetOwner(), FName("ShieldArm"));
	if (ShieldArm)
	{
		ShieldArm->SetupAttachment(GetOwner()->GetRootComponent());
		ShieldArm->RegisterComponent();
		ShieldArm->TargetArmLength = 15000.0f;
		//ShieldArm->SetIsReplicated(true);
	}

	// Setup mesh, local players only. I had to check on the controller itself and not on the ship because OwningShip->IsLocallyControlled() returns
	// false during BeginPlay for listen server hosts, and that's fucking retarded.
	// https://answers.unrealengine.com/questions/713607/is-locally-controlled-returns-false-for-listen-ser.html
	APlayerController* const OwningPlayerController = Cast<APlayerController>(OwningShip->GetOwner());
	if (OwningPlayerController)
	{
		if (OwningPlayerController->IsLocalController())
		{
			SetupMesh = NewObject<UStaticMeshComponent>(GetOwner(), FName("SetupMesh"));
			if (SetupMesh)
			{
				SetupMesh->SetupAttachment(ShieldArm);
				SetupMesh->RegisterComponent();
				SetupMesh->SetCollisionProfileName("NoCollision");
				SetupMesh->SetStaticMesh(DataManager->ShieldSettings.ShieldMesh);
				SetupMesh->SetMaterial(0, DataManager->ShieldSettings.SetupMaterial);
				SetupMesh->SetRelativeLocation(FVector(DataManager->ShieldSettings.ShieldMeshDefaultSize.Z * DataManager->ShieldSettings.ShieldMeshScale.Z + 500.0f, 0.0f, 0.0f));
				SetupMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
				SetupMesh->SetRelativeScale3D(DataManager->ShieldSettings.ShieldMeshScale);
				SetupMesh->SetHiddenInGame(true);
			}
		}
	}
	
	// Shield actor
	ShieldActor = GetWorld()->SpawnActorDeferred<ASB_Shield>(DataManager->ShieldSettings.ShieldBP, FTransform(FVector()), GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (ShieldActor)
	{
		UGameplayStatics::FinishSpawningActor(ShieldActor, FTransform(FVector()));
		ShieldActor->AttachToComponent(ShieldArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ShieldActor->SetActorRelativeLocation(FVector(DataManager->ShieldSettings.ShieldMeshDefaultSize.Z * DataManager->ShieldSettings.ShieldMeshScale.Z + 500.0f, 0.0f, 0.0f));
		ShieldActor->SetActorRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		ShieldActor->SetActorHiddenInGame(true);
	}

	ShieldDurability = DataManager->ShieldSettings.MaxDurability;
}*/

void USB_ShieldModule::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetWorld()->IsGameWorld() == false)
		return;
}

void USB_ShieldModule::BeginPlay()
{
	Super::BeginPlay();
}

void USB_ShieldModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSetupMode)
	{
		ShieldArm->SetWorldRotation(FRotator(0.0f, OwningShip->GetShipCameraManager()->GetCameraArmCT()->GetComponentRotation().Yaw - 180.0f, 0.0f)); // ToDo : Dirty af :x
	}

	if (ShieldState == ESB_ShieldState::Deployed)
	{
		ShieldArm->SetWorldRotation(FRotator(0.0f, DeployedRotationYaw, 0.0f));
	}

	if (ShieldState == ESB_ShieldState::Cooldown)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		if ((CurrentTime - LastUndeployTime) >= DataManager->ShieldSettings.DeployCooldown)
		{
			ShieldState = ESB_ShieldState::Ready;
		}

		OnShieldCooldownUpdated.Broadcast(CurrentTime - LastUndeployTime, DataManager->ShieldSettings.DeployCooldown); //
	}
}

void USB_ShieldModule::StartSetup()
{
	if (bIsSetupMode == false && ShieldState == ESB_ShieldState::Ready)
	{
		bIsSetupMode = true;
		SetupMesh->SetHiddenInGame(false);
	}
}

void USB_ShieldModule::StopSetup()
{
	if (bIsSetupMode == true)
	{
		bIsSetupMode = false;
		SetupMesh->SetHiddenInGame(true);
	}
}

void USB_ShieldModule::Deploy()
{
	if (ShieldState != ESB_ShieldState::Ready)
		return;

	if (OwningShip->GetLocalRole() < ROLE_Authority)
		Deploy_Server(ShieldArm->GetComponentRotation().Yaw);
	else
		Deploy_Multicast(ShieldArm->GetComponentRotation().Yaw);
}

void USB_ShieldModule::Deploy_Server_Implementation(float NewDeployedRotationYaw)
{
	if (ShieldState != ESB_ShieldState::Ready)
		return;

	Deploy_Multicast(NewDeployedRotationYaw);
}

void USB_ShieldModule::Deploy_Multicast_Implementation(float NewDeployedRotationYaw)
{
	if (SetupMesh)
		SetupMesh->SetHiddenInGame(true);
	
	bIsSetupMode = false;
	DeployedRotationYaw = NewDeployedRotationYaw;
	ShieldActor->SetActorHiddenInGame(false);
	ShieldActor->OnDeployedBPN();
	ShieldState = ESB_ShieldState::Deployed;
}

void USB_ShieldModule::Undeploy()
{
	if (ShieldState != ESB_ShieldState::Deployed)
		return;
	
	if (OwningShip->GetLocalRole() < ROLE_Authority)
		Undeploy_Server();
	else
		Undeploy_Multicast();
}

void USB_ShieldModule::Undeploy_Server_Implementation()
{
	if (ShieldState != ESB_ShieldState::Deployed)
		return;
	
	Undeploy_Multicast();
}

void USB_ShieldModule::Undeploy_Multicast_Implementation()
{
	ShieldActor->SetActorHiddenInGame(true);
	ShieldActor->OnDestroyedBPN();
	ShieldState = ESB_ShieldState::Cooldown;
	LastUndeployTime = GetWorld()->GetTimeSeconds();
}

void USB_ShieldModule::ApplyShieldDamage(float Damage, const FVector& HitLocation,  AController* const InstigatorController)
{
	ShieldDurability = FMath::Clamp(ShieldDurability - Damage, 0.0f, DataManager->ShieldSettings.MaxDurability);
	GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTimer);
	GetWorld()->GetTimerManager().SetTimer(ShieldRegenTimer, this, &USB_ShieldModule::RegenOnce, DataManager->ShieldSettings.RegenRate, true, DataManager->ShieldSettings.RegenRate);

	if (ShieldDurability == 0)
		Undeploy();
	
	ASB_PlayerController* const InstigatorPlayerController = Cast<ASB_PlayerController>(InstigatorController);
	if (InstigatorPlayerController)
	{
		InstigatorPlayerController->OnDamageDealt(Damage, 0.0f, HitLocation, ESB_PrimaryDamageType::Shield);
	}
	
	OnShieldDurabilityUpdated.Broadcast(ShieldDurability, DataManager->ShieldSettings.MaxDurability);
}

void USB_ShieldModule::RegenOnce()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, "USB_ShieldModule::RegenOnce");
	
	ShieldDurability = FMath::Clamp(ShieldDurability + DataManager->ShieldSettings.RegenAmount, 0.0f, DataManager->ShieldSettings.MaxDurability);

	if (ShieldDurability == DataManager->ShieldSettings.MaxDurability)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, "USB_ShieldModule::RegenOnce - Invalidate");
		GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTimer);
	}

	OnShieldDurabilityUpdated.Broadcast(ShieldDurability, DataManager->ShieldSettings.MaxDurability);
}

void USB_ShieldModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_ShieldModule, ShieldDurability);
}

void USB_ShieldModule::OnRep_ShieldDurability() const
{
	OnShieldDurabilityUpdated.Broadcast(ShieldDurability, DataManager->ShieldSettings.MaxDurability);
}

