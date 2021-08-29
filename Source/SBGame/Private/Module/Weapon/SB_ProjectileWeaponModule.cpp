#include "Module/Weapon/SB_ProjectileWeaponModule.h"
#include "Module/Weapon/SB_Projectile.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_TargetPoint.h"
#include "SB_GameState.h"
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
}

void USB_ProjectileWeaponModule::UpdateFire()
{
	if (GetOwnerRole() < ROLE_Authority)
		return;

	if (WeaponModuleData == nullptr ||
		GState->GameType != ESB_GameType::Battle ||
		GState->GamePhase != ESB_GamePhase::Playing)
		return;

	// If facing enemy vehicle and rotation <= precision, set bWantsToFire = true.
	
	bWantsToFire = false;
	
	if (TargetPoint.IsValid())
	{
		/*const float CurrentRotationYaw = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetPoint->GetComponentLocation()).Yaw;

		FRotator DeltaR = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetPoint->GetComponentLocation()), LerpedRotation);
		RZ_UtilityLibrary::PrintFloatToScreen("USB_ProjectileWeaponModule::UpdateFire - DeltaR : ", DeltaR.Yaw, FColor::Green, -1, GetWorld()->GetDeltaSeconds());
		
		if (DeltaR.Yaw <= 5.0f &&
		DeltaR.Yaw >= -5.0f)
		{

			bWantsToFire = true;
		}*/

		const FRotator CurrentWorldRotation = UKismetMathLibrary::TransformRotation(GetOwner()->GetTransform(), CurrentRotation);
		const FRotator ConvertedCurrentWorldRotation = FRotator(CurrentWorldRotation.Pitch, CurrentWorldRotation.Yaw + 90.0f, CurrentWorldRotation.Roll);
		const FRotator TargetWorldRotation = UKismetMathLibrary::FindLookAtRotation(GetSocketLocation("ViewSocket"), TargetPoint->GetComponentLocation());
		const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ConvertedCurrentWorldRotation, TargetWorldRotation);
		
		RZ_UtilityLibrary::PrintFloatToScreen("USB_ProjectileWeaponModule::UpdateFire - DeltaR : ", DeltaRotation.Yaw, FColor::Green, -1, GetWorld()->GetDeltaSeconds());
		
		if (DeltaRotation.Yaw <= 5.0f && DeltaRotation.Yaw >= -5.0f)
		{
			bWantsToFire = true;
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

		// Projectile
		
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
	}
}
