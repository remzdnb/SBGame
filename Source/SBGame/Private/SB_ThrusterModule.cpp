#include "SB_ThrusterModule.h"
#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_DataManager.h"
//
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

USB_ThrusterModule::USB_ThrusterModule()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ThrusterModule::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetWorld()->IsGameWorld() == false)
		return;

	ShipMovementCT = Cast<ASB_Ship>(GetOwner())->GetShipMovementCT();
}

void USB_ThrusterModule::BeginPlay()
{
	Super::BeginPlay();

	ThrusterModuleData = DataManager->GetThrusterModuleDataFromRow(ModuleName);
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

void USB_ThrusterModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateExhaustParticle();
	Debug(DeltaTime);
}

void USB_ThrusterModule::UpdateExhaustParticle()
{
	if (ShipMovementCT == nullptr)
		return;

	if (SlotName == "Left Thruster")
	{
		if (ShipMovementCT->GetRightAxisValue() < 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
	else if (SlotName == "Right Thruster")
	{
		if (ShipMovementCT->GetRightAxisValue() > 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
	else if (SlotName == "Back Thruster")
	{
		if (ShipMovementCT->GetForwardAxisValue() > 0)
		{
			SetExhaustParticlesVisibility(true);
		}
		else
		{
			SetExhaustParticlesVisibility(false);
		}
	}
	else if (SlotName == "Front Thruster")
	{
		if (ShipMovementCT->GetForwardAxisValue() < 0)
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
	
	if (DataManager->GameSettings.bIsDebugEnabled_ThrusterModule == false || OwnerShip == nullptr || ShipMovementCT == nullptr)
		return;

	FString RoleString = "None";
	FColor Color = FColor::White;

	if (OwnerShip->GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (OwnerShip->GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (OwnerShip->GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy // ";
		Color = FColor::Orange;
	}

	FString StringToPrint;

	if (ExhaustParticles.Num() == 0)
		StringToPrint = RoleString + this->GetName() + " // Exhaust Particle : list empty // FAxis : " + FString::FromInt(ShipMovementCT->GetForwardAxisValue());
	else
		StringToPrint = RoleString + this->GetName() + " // Exhaust Particle : list filled // FAxis : " + FString::FromInt(ShipMovementCT->GetForwardAxisValue());

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, StringToPrint);
}
