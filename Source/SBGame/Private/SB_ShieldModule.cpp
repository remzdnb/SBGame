#include "SB_ShieldModule.h"
#include "SB_Shield.h"
#include "SB_Ship.h"
#include "SB_ShipCameraManager.h"
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
	bIsDeployed = false;
}

void USB_ShieldModule::BeginPlay()
{
	Super::BeginPlay();

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
}

void USB_ShieldModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSetupMode)
	{
		ShieldArm->SetWorldRotation(FRotator(0.0f, OwningShip->GetShipCameraManager()->GetCameraArmCT()->GetComponentRotation().Yaw - 180.0f, 0.0f)); // ToDo : Dirty af :x
	}

	if (bIsDeployed)
	{
		ShieldArm->SetWorldRotation(FRotator(0.0f, DeployedRotationYaw, 0.0f));
	}
}

void USB_ShieldModule::StartSetup()
{
	bIsSetupMode = true;
	SetupMesh->SetHiddenInGame(false);
}

void USB_ShieldModule::StopSetup()
{
	bIsSetupMode = false;
	SetupMesh->SetHiddenInGame(true);
}

void USB_ShieldModule::StartDeploy()
{
	if (OwningShip->IsLocallyControlled() == false)
		return;
	
	Deploy(ShieldArm->GetComponentRotation().Yaw);
}

void USB_ShieldModule::Deploy(const float NewDeployedRotationYaw)
{
	if (OwningShip->GetLocalRole() < ROLE_Authority)
		Deploy_Server(NewDeployedRotationYaw);
	else
		Deploy_Multicast(NewDeployedRotationYaw);
}

void USB_ShieldModule::Deploy_Server_Implementation(const float NewDeployedRotationYaw)
{
	Deploy_Multicast(NewDeployedRotationYaw);
}

void USB_ShieldModule::Deploy_Multicast_Implementation(const float NewDeployedRotationYaw)
{
	if (SetupMesh)
		SetupMesh->SetHiddenInGame(true);
	
	bIsSetupMode = false;
	bIsDeployed = true;
	DeployedRotationYaw = NewDeployedRotationYaw;
	ShieldActor->SetActorHiddenInGame(false);
	ShieldActor->OnDeployedBPN();
}

void USB_ShieldModule::Undeploy()
{
	if (OwningShip->GetLocalRole() < ROLE_Authority)
		Undeploy_Server();
	else
		Undeploy_Multicast();
}

void USB_ShieldModule::Undeploy_Server_Implementation()
{
	Undeploy_Multicast();
}

void USB_ShieldModule::Undeploy_Multicast_Implementation()
{
	ShieldActor->SetActorHiddenInGame(true);
	ShieldActor->OnDestroyedBPN(); // eh
	bIsDeployed = false;
}

void USB_ShieldModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_ShieldModule, ShieldDurability);
	//DOREPLIFETIME(USB_ShieldModule, DeployedRotationYaw);
}

void USB_ShieldModule::OnRep_ShieldDurability() const
{
	//OnDurabilityUpdated.Broadcast(Durability, DataManager->ShieldSettings.MaxDurability);
}

