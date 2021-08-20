#include "Module/Weapon/SB_TraceWeaponModule.h"
//
#include "Kismet/GameplayStatics.h"

USB_TraceWeaponModule::USB_TraceWeaponModule()
{
	bIsFiring = false;
	LastFireTime = 0.0f;
}

void USB_TraceWeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFire();
}

void USB_TraceWeaponModule::UpdateFire()
{
	if (GetOwnerRole() < ROLE_Authority)
		return;

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if ((CurrentTime - LastFireTime) >= WeaponModuleData->FireRate)
	{
		if (bIsFiring)
		{
			bIsFiring = false;
		}
	}

	if (bWantsToFire && !bIsFiring)
	{
		FireOnce();
		bIsFiring = true;
		LastFireTime = CurrentTime;
	}
}

void USB_TraceWeaponModule::FireOnce()
{
	// Fire
	for (uint8 MuzzleIndex = 1; MuzzleIndex <= WeaponModuleData->MuzzleCount; MuzzleIndex++)
	{
		const FName MuzzleSocketName = *("MuzzleSocket_0" + FString::FromInt(MuzzleIndex));
		const FVector StartLocation = GetSocketLocation(MuzzleSocketName);
		const FVector EndLocation = StartLocation + GetSocketRotation(MuzzleSocketName).Vector() * 9999999999.0f;
		FCollisionQueryParams TraceParams;
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			WeaponModuleData->ImpactParticle,
			HitResult.Location,
			FRotator(0.0f),
			FVector(WeaponModuleData->ImpactParticleScale),
			true
		);
	}

	// Muzzle FX
	for (uint8 MuzzleIndex = 1; MuzzleIndex <= WeaponModuleData->MuzzleCount; MuzzleIndex++)
	{
		const FName MuzzleSocketName = *("MuzzleSocket_0" + FString::FromInt(MuzzleIndex));
		
		if (WeaponModuleData->MuzzleParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(
				WeaponModuleData->MuzzleParticle,
				this,
				MuzzleSocketName,
				FVector(0.0f),
				FRotator(0.0f),
				FVector(WeaponModuleData->MuzzleParticleScale),
				EAttachLocation::SnapToTarget
			);
		}
	}
}
