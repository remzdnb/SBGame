#include "SB_ShieldModule.h"
#include "SB_Shield.h"
#include "SB_Ship.h"
#include "SB_ShipCameraComponent.h"
#include "SB_DataManager.h"
//
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

USB_ShieldModule::USB_ShieldModule()
{
	SetCollisionProfileName("CharacterMesh");

	PrimaryComponentTick.bCanEverTick = true;

	bIsSetupMode = false;
	bIsDeployed = false;
}

void USB_ShieldModule::BeginPlay()
{
	Super::BeginPlay();

	/*ShieldArmCT = NewObject<USpringArmComponent>(GetOwner(), FName("ShieldArmCT"));
	if (ShieldArmCT)
	{
		ShieldArmCT->SetupAttachment(GetOwner()->GetRootComponent());
		ShieldArmCT->RegisterComponent();
		ShieldArmCT->TargetArmLength = 7000.0f;
	}

	ShieldActor = GetWorld()->SpawnActorDeferred<ASB_Shield>(DataManager->ShieldSettings.ShieldClass, FTransform(FVector()), GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (ShieldActor)
	{
		ShieldActor->Init(DataManager);
		UGameplayStatics::FinishSpawningActor(ShieldActor, FTransform(FVector()));
		ShieldActor->AttachToComponent(ShieldArmCT, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}*/
}

void USB_ShieldModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (bIsSetupMode)
	{
		ShieldArmCT->SetWorldRotation(FRotator(0.0f, OwnerShip->GetShipCameraCT()->GetCameraArmCT()->GetComponentRotation().Yaw - 180.0f, 0.0f)); // ToDo : Dirty af :x
	}

	if (bIsDeployed)
	{
		ShieldArmCT->SetWorldRotation(FRotator(0.0f, DeployedRotation.Yaw, 0.0f));
	}*/
}

void USB_ShieldModule::StartSetup()
{
	if (ShieldActor == nullptr)
		return;

	if (bIsDeployed)
	{
		bIsDeployed = false;
	}

	bIsSetupMode = true;
	ShieldActor->OnSetupStart();
}

void USB_ShieldModule::StopSetup()
{
	if (ShieldActor == nullptr)
		return;

	if (bIsSetupMode)
	{
		bIsSetupMode = false;
		ShieldActor->OnSetupStop();
	}
}

void USB_ShieldModule::Deploy()
{
	ShieldActor->OnDeployedBPN();
	DeployedRotation = ShieldArmCT->GetComponentRotation();
	bIsSetupMode = false;
	bIsDeployed = true;
}