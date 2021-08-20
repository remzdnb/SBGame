#include "Module/Weapon/SB_BeamWeaponModule.h"
#include "Module/Weapon/SB_Beam.h"
//
#include "Particles/ParticleSystemComponent.h" 
#include "Kismet/GameplayStatics.h"

void USB_BeamWeaponModule::Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName)
{
	Super::Init(NewModuleSlotData, NewModuleRowName);

	/*BeamActor = GetWorld()->SpawnActorDeferred<ASB_Beam>(WeaponModuleData->BeamBP, FTransform(FVector()), GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (BeamActor)
	{
		UGameplayStatics::FinishSpawningActor(BeamActor, FTransform(FVector()));
		BeamActor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "MuzzleSocket_01");
		BeamActor->SetActorScale3D(WeaponModuleData->BeamScale);
	}*/

	const FString ComponentName = this->GetName() + "_BeamParticle";
	BeamParticle = NewObject<UParticleSystemComponent>(this, *ComponentName);
	BeamParticle->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "MuzzleSocket_01");
	BeamParticle->SetTemplate(WeaponModuleData->BeamParticle);
	BeamParticle->SetWorldScale3D(FVector(1.0f));
	BeamParticle->RegisterComponent();

	//

	CurrentBeamLength = 0.0f;
}

void USB_BeamWeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bWantsToFire)
		IncreaseBeamLength(DeltaTime);
	else
		DecreaseBeamLength(DeltaTime);

	BeamParticle->SetVectorParameter("length", FVector(1.0f, 1.0f, CurrentBeamLength));
	BeamParticle->SetVectorParameter("beamEnd", FVector(CurrentBeamLength, 0.0f, 0.0f));
	BeamParticle->SetFloatParameter("tileVcorrect", CurrentBeamLength / 20.0f / 25.0f);
}

void USB_BeamWeaponModule::SetWantsToFire(bool bNewWantsToFire)
{
	Super::SetWantsToFire(bNewWantsToFire);

	/*if (bNewWantsToFire)
		StartBeam();
	else
		StopBeam();*/


}

void USB_BeamWeaponModule::IncreaseBeamLength(float DeltaTime)
{
	if (CurrentBeamLength < WeaponModuleData->Range)
	{
		CurrentBeamLength = CurrentBeamLength + WeaponModuleData->BeamSpeed * DeltaTime;
	}
}

void USB_BeamWeaponModule::DecreaseBeamLength(float DeltaTime)
{
	if (CurrentBeamLength > 0)
	{
		CurrentBeamLength = CurrentBeamLength - WeaponModuleData->BeamSpeed * DeltaTime;
	}
}

void USB_BeamWeaponModule::CheckBeamCollision()
{
	const FVector Start = GetSocketLocation("MuzzleSocket_01");
	const FVector End =
		GetSocketLocation("MuzzleSocket_01") +
		GetSocketRotation("MuzzleSocket_01").Vector()
		* CurrentBeamLength;

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredComponent(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	/*if (HitResult.Actor == TargetShip)
	{
		if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Green, 0.3f, 20.0f);

		TargetPoint = ThisTargetPoint;
		bWantsToFire = true;
		return;
	}

	if (GInstance->DebugSettings.bIsDebugEnabled_WeaponModule)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor::Red, TARGETUPDATERATE - 0.2f, 20.0f);
	}*/
}

