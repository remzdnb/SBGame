#include "Module/Weapon/SB_BeamWeaponModule.h"
#include "Module/Weapon/SB_Beam.h"
#include "Vehicle/SB_Vehicle.h"
#include "SB_GameInstance.h"
#include "SB_PlayerState.h"
//
#include "SB_GameState.h"
#include "Particles/ParticleSystemComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

void USB_BeamWeaponModule::Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName)
{
	Super::Init(NewModuleSlotData, NewModuleRowName);

	const ASB_Vehicle* OwningVehicle = Cast<ASB_Vehicle>(GetOwner());
	OwnerController = Cast<AController>(OwningVehicle->GetOwner());

	if (GState->GameType == ESB_GameType::Battle)
	{
		const FString ComponentName = this->GetName() + "_BeamParticle";
		BeamParticle = NewObject<UParticleSystemComponent>(this, *ComponentName);
		BeamParticle->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "MuzzleSocket_01");
		BeamParticle->SetTemplate(WeaponModuleData->BeamParticle);
		BeamParticle->SetWorldScale3D(FVector(1.0f));
		BeamParticle->SetVisibility(false);
		BeamParticle->RegisterComponent();
	}
}

void USB_BeamWeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GState->GameType == ESB_GameType::Battle)
	{
		CheckBeamCollision();
	}
	
	//BeamParticle->SetVectorParameter("beamEnd", FVector(CurrentBeamLength, 0.0f, 0.0f));
	//BeamParticle->SetFloatParameter("tileVcorrect", CurrentBeamLength / 20.0f / 25.0f);
}

void USB_BeamWeaponModule::SetWantsToFire(bool bNewWantsToFire)
{
	Super::SetWantsToFire(bNewWantsToFire);
}

void USB_BeamWeaponModule::CheckBeamCollision()
{
	const FVector Start = GetSocketLocation("ViewSocket");
	const FVector End =
		GetSocketLocation("ViewSocket") +
		GetSocketRotation("ViewSocket").Vector() *
		WeaponModuleData->Range;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredComponent(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
	ASB_Vehicle* HitVehicle = Cast<ASB_Vehicle>(HitResult.Actor);
	if (HitVehicle)
	{
		const ASB_PlayerState* const SelfPlayerState = Cast<ASB_PlayerState>(Cast<APawn>(GetOwner())->GetPlayerState());
		const ASB_PlayerState* const TargetPlayerState = Cast<ASB_PlayerState>(HitVehicle->GetPlayerState());
		if (SelfPlayerState && TargetPlayerState)
		{
			if (SelfPlayerState->GetTeamID() != TargetPlayerState->GetTeamID())
			{
				UpdateBeam();

				if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
				{
					UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Purple, GetWorld()->GetDeltaSeconds() + 0.01f, 50.0f);
				}

				return;
			}
		}
	}
	
	StopBeam();

	if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, GetWorld()->GetDeltaSeconds() + 0.01f, 50.0f);
	}
}

void USB_BeamWeaponModule::UpdateBeam()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DamageTimer) == false)
	{
		if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule == false)
		{
			//BeamParticle->SetVisibility(true);
		}

		BeamParticle->SetVisibility(true);
		
		GetWorld()->GetTimerManager().SetTimer(
			DamageTimer,
			this,
			&USB_BeamWeaponModule::ApplyBeamDamage,
			WeaponModuleData->BeamTickRate,
			true,
			WeaponModuleData->BeamTickRate
		);
	}

	// doesnt work well, think about it
	/*BeamParticle->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(
			GetSocketLocation("MuzzleSocket_01"),
			HitResult.Location)
	);*/
	const float NewBeamLength = FVector::Dist(GetSocketLocation("MuzzleSocket_01"), HitResult.Location);
	BeamParticle->SetVectorParameter("length", FVector(1.0f, 1.0f, NewBeamLength));
}

void USB_BeamWeaponModule::StopBeam()
{
	BeamParticle->SetVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
}

void USB_BeamWeaponModule::ApplyBeamDamage()
{
	ISB_CombatInterface* CombatInterface = Cast<ISB_CombatInterface>(HitResult.Component);
	if (CombatInterface)
	{
		CombatInterface->ApplyDamageFromProjectile(WeaponModuleData->Damage, HitResult.Location, OwnerController);
	}
}

