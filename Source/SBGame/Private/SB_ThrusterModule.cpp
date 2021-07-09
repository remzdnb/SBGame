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

void USB_ThrusterModule::Init(const class ASB_DataManager* const NewDataManager, const FName& NewParentSlotName, const FName& NewDataRowName)
{
	Super::Init(NewDataManager, NewParentSlotName, NewDataRowName);

	ThrusterModuleData = NewDataManager->GetThrusterModuleDataFromRow(NewDataRowName);
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

	ShipMovementCT = Cast<ASB_Ship>(GetOwner())->GetShipMovementCT();
}

void USB_ThrusterModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ParentSlotName == "LeftThruster")
	{
		if (ShipMovementCT->GetRightAxisValue() < 0)
		{
			SetExhaustParticlesVisibility(false);
		}
		else
		{
			SetExhaustParticlesVisibility(true);
		}
	}
	else if (ParentSlotName == "RightThruster")
	{
		if (ShipMovementCT->GetRightAxisValue() > 0)
		{
			SetExhaustParticlesVisibility(false);
		}
		else
		{
			SetExhaustParticlesVisibility(true);
		}
	}
	else if (ParentSlotName == "BackThruster")
	{
		if (ShipMovementCT->GetForwardAxisValue() != 0)
		{
			SetExhaustParticlesVisibility(false);
		}
		else
		{
			SetExhaustParticlesVisibility(true);
		}
	}

	Debug(DeltaTime);
}

void USB_ThrusterModule::SetExhaustParticlesVisibility(bool bNewIsVisible)
{
	if (bNewIsVisible)
	{
		for (auto& ExhaustParticle : ExhaustParticles)
		{
			ExhaustParticle->SetHiddenInGame(true);
		}
	}
	else
	{
		for (auto& ExhaustParticle : ExhaustParticles)
		{
			ExhaustParticle->SetHiddenInGame(false);
		}
	}
}

void USB_ThrusterModule::Debug(float DeltaTime)
{
	if (DataManager->GameSettings.bIsDebugEnabled_ThrusterModule == false || OwnerShip == nullptr || ParentSlotName != "BackThruster")
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
