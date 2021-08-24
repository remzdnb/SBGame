#include "Module/SB_ThrusterModule.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_ShipMovementComponent.h"
#include "SB_GameInstance.h"
//
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

USB_ThrusterModule::USB_ThrusterModule()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ThrusterModule::Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName)
{
	Super::Init(NewModuleSlotData, NewModuleRowName);

	ThrusterModuleData = GInstance->GetThrusterModuleDataFromRow(ModuleRowName);
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

	ShipMovement = Cast<ASB_Vehicle>(GetOwner())->GetShipMovement();
}

void USB_ThrusterModule::BeginPlay()
{
	Super::BeginPlay();
}

void USB_ThrusterModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateExhaustParticle();
	
	//Debug(DeltaTime);
}

void USB_ThrusterModule::UpdateState(const ESB_ModuleState NewState)
{
	Super::UpdateState(NewState);

	/*if (State == ESB_ModuleState::Repairing)
	{
		
	}
	else
	{
		
	}*/
}

void USB_ThrusterModule::UpdateExhaustParticle()
{
	if (ShipMovement == nullptr)
		return;

	if (ModuleSlotData.Tags.Contains("LeftThruster"))
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
	else if (ModuleSlotData.Tags.Contains("RightThruster"))
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
	else if (ModuleSlotData.Tags.Contains("BackThruster"))
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
	else if (ModuleSlotData.Tags.Contains("FrontThruster"))
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
	for (const auto& ExhaustParticle : ExhaustParticles)
	{
		ExhaustParticle->SetHiddenInGame(!bNewIsVisible);
	}
}

void USB_ThrusterModule::Debug(float DeltaTime)
{
	if (ShipMovement == nullptr)
		return;

	FString RoleString = "None";
	FColor Color = FColor::White;

	if (GetOwnerRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (GetOwnerRole() == ROLE_SimulatedProxy)
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
