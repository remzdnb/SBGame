#include "Module/SB_ThrusterModule.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "SB_DataManager.h"
//
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

USB_ThrusterModule::USB_ThrusterModule()
{
	//UPrimitiveComponent::SetCollisionProfileName("CharacterMesh");
	
	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ThrusterModule::Init(
	const ASB_DataManager* const NewDataManager, 
	const FSB_ModuleSlotData* const NewModuleSlotData, 
	const FName& NewModuleRowName)
{
	Super::Init(NewDataManager, NewModuleSlotData, NewModuleRowName);

	ThrusterModuleData = DataManager->GetThrusterModuleDataFromRow(ModuleRowName);
	if (ThrusterModuleData)
	{
		for (auto& ExhaustSocketName : ThrusterModuleData->ExhaustSocketNames)
		{
			UParticleSystemComponent* const NewExhaustParticle = UGameplayStatics::SpawnEmitterAttached(
				ThrusterModuleData->ExhaustParticle,
				this,
				ExhaustSocketName, 
				FVector(0.0f), 
				FRotator(0.0f), 
				EAttachLocation::SnapToTarget
			);

			if (NewExhaustParticle)
			{
				NewExhaustParticle->SetWorldScale3D(FVector(ThrusterModuleData->ExhaustParticleScale));
				NewExhaustParticle->SetHiddenInGame(true);
				ExhaustParticles.Add(NewExhaustParticle);
			}
		}
	}
}

void USB_ThrusterModule::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetWorld()->IsGameWorld() == false)
		return;

	ShipMovement = Cast<ASB_Ship>(GetOwner())->GetShipMovement();
}

void USB_ThrusterModule::BeginPlay()
{
	Super::BeginPlay();
}

void USB_ThrusterModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateExhaustParticle();
	Debug(DeltaTime);
}

void USB_ThrusterModule::UpdateState(const ESB_ModuleState NewState)
{
	Super::UpdateState(NewState);

	if (State == ESB_ModuleState::Repairing)
	{
		
	}
	else
	{
		
	}
}

void USB_ThrusterModule::UpdateExhaustParticle()
{
	if (ModuleSlotData == nullptr || ShipMovement == nullptr)
		return;

	if (ModuleSlotData->Type == ESB_ModuleSlotType::Thruster_Left)
	{
		if (ShipMovement->GetRightAxisValue() < 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
	else if (ModuleSlotData->Type == ESB_ModuleSlotType::Thruster_Right)
	{
		if (ShipMovement->GetRightAxisValue() > 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
	else if (ModuleSlotData->Type == ESB_ModuleSlotType::Thruster_Back)
	{
		if (ShipMovement->GetForwardAxisValue() > 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
	else if (ModuleSlotData->Type == ESB_ModuleSlotType::Thruster_Front)
	{
		if (ShipMovement->GetForwardAxisValue() < 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
}

void USB_ThrusterModule::SetExhaustParticlesVisibility(bool bNewIsVisible)
{
	for (auto& ExhaustParticle : ExhaustParticles)
	{
		ExhaustParticle->SetHiddenInGame(!bNewIsVisible);
	}
}

void USB_ThrusterModule::Debug(float DeltaTime)
{
	if (DataManager == nullptr)
		return;
	
	if (DataManager->GameSettings.bIsDebugEnabled_ThrusterModule == false || OwningShip == nullptr || ShipMovement == nullptr)
		return;

	FString RoleString = "None";
	FColor Color = FColor::White;

	if (OwningShip->GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (OwningShip->GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (OwningShip->GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy // ";
		Color = FColor::Orange;
	}

	FString StringToPrint;

	if (ExhaustParticles.Num() == 0)
		StringToPrint = RoleString + this->GetName() + " // Exhaust Particle : list empty // FAxis : " + FString::FromInt(ShipMovement->GetForwardAxisValue());
	else
		StringToPrint = RoleString + this->GetName() + " // Exhaust Particle : list filled // FAxis : " + FString::FromInt(ShipMovement->GetForwardAxisValue());

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, StringToPrint);
}
