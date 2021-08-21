#include "Module/Weapon/SB_ProjectileWeaponModule.h"
#include "Module/Weapon/SB_Projectile.h"
#include "Vehicle/SB_TargetPoint.h"
//
#include "RZ_UtilityLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

USB_ProjectileWeaponModule::USB_ProjectileWeaponModule()
{
	bIsFiring = false;
	LastFireTime = 0.0f;
}

void USB_ProjectileWeaponModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFire();

	if (TargetPoint.IsValid())
	{
		//RZ_UtilityLibrary::PrintFloatToScreen("USB_ProjectileWeaponModule::UpdateFire - Target Rotation : ", UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetPoint->GetComponentLocation()).Yaw, FColor::Green, -1, DeltaTime);
		//RZ_UtilityLibrary::PrintFloatToScreen("USB_ProjectileWeaponModule::UpdateFire - Current Rotation : ", LerpedRotation.Yaw, FColor::Orange, -1, DeltaTime);
	}
}

void USB_ProjectileWeaponModule::UpdateFire()
{
	if (GetOwnerRole() < ROLE_Authority)
		return;

	SetWantsToFire(false);
	
	if (TargetPoint.IsValid())
	{
		const float CurrentRotationYaw = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetPoint->GetComponentLocation()).Yaw;

		FRotator DeltaR = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetPoint->GetComponentLocation()), LerpedRotation);
		RZ_UtilityLibrary::PrintFloatToScreen("USB_ProjectileWeaponModule::UpdateFire - DeltaR : ", DeltaR.Yaw, FColor::Green, -1, GetWorld()->GetDeltaSeconds());
		
		if (DeltaR.Yaw <= 5.0f &&
		DeltaR.Yaw >= -5.0f)
		{
			//RZ_UtilityLibrary::PrintStringToScreen("USB_ProjectileWeaponModule::UpdateFire - RotationYawDiff : ", "oui", FColor::Green, -1, 0.5f);
			SetWantsToFire(true);
		}
	}
	
	//

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


void USB_ProjectileWeaponModule::FireOnce()
{
	for (uint8 MuzzleIndex = 1; MuzzleIndex <= WeaponModuleData->MuzzleCount; MuzzleIndex++)
	{
		const FName MuzzleSocketName = *("MuzzleSocket_0" + FString::FromInt(MuzzleIndex));

		//Projectile
		const FVector StartLocation = GetSocketLocation(MuzzleSocketName);
		const FVector TargetLocation = TargetPoint->GetComponentLocation();
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		const FTransform SpawnTransform(SpawnRotation, StartLocation, FVector(1.0f));
		const FActorSpawnParameters SpawnParameters;
		ASB_Projectile* const Projectile = GetWorld()->SpawnActorDeferred<ASB_Projectile>(WeaponModuleData->ProjectileBP, SpawnTransform, GetOwner(), nullptr);
		if (Projectile)
		{
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
		}
		
		// Muzzle FX
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
