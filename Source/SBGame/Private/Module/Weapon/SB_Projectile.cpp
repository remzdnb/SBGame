#include "Module/Weapon/SB_Projectile.h"
#include "Vehicle/SB_Vehicle.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameInstance.h"
#include "SB_Interfaces.h"
//
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASB_Projectile::ASB_Projectile()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphere"));
	CollisionSphere->InitSphereRadius(1.0f);
	CollisionSphere->SetCollisionProfileName("Projectile");
	CollisionSphere->bTraceComplexOnMove = true;
	CollisionSphere->SetGenerateOverlapEvents(false);
	CollisionSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = CollisionSphere;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("VisualMesh"));
	VisualMesh->SetCollisionProfileName("NoCollision");
	VisualMesh->bTraceComplexOnMove = false;
	VisualMesh->SetGenerateOverlapEvents(false);
	VisualMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	VisualMesh->SetupAttachment(RootComponent);

	VisualParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("VisualParticle"));
	VisualParticle->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ASB_Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	WeaponModuleData = GInstance->GetWeaponModuleDataFromRow(DataRowName);
	
	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionSphere->OnComponentHit.AddDynamic(this, &ASB_Projectile::OnHit);
	}
	
	SetLifeSpan(10.0f);
}

void ASB_Projectile::BeginPlay()
{
	Super::BeginPlay();

	OwningVehicle = Cast<ASB_Vehicle>(GetOwner());
	if (OwningVehicle.IsValid())
	{
		OwningController = Cast<AController>(OwningVehicle->GetOwner());
		CollisionSphere->IgnoreActorWhenMoving(OwningVehicle.Get(), true);
	}
}

void ASB_Projectile::Destroyed()
{
	if (GetWorld()->IsGameWorld() == true)
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileData->ImpactParticle, GetActorLocation(), FRotator(), FVector(ProjectileData->ImpactParticleScale), true, EPSCPoolMethod::None, true);
	}

	Super::Destroyed();
}

void ASB_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetOwner())
	{
		// Combat interface is implemented both on shields/drones (actors) and on ship modules (components).
		ISB_CombatInterface* CombatInterface = Cast<ISB_CombatInterface>(OtherComp);
		if (CombatInterface)
		{
			CombatInterface->ApplyDamageFromProjectile(WeaponModuleData->Damage, Hit.Location, OwningController.Get());
		}
		
		//Debug(OtherActor);
		//const FString StringToPrint = "ASB_Projectile HIT - Projectile destroyed on Actor : " + OtherActor->GetName() + "// Component : " + OtherComp->GetName();

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, StringToPrint);
	}

	SpawnImpactFX_Multicast(OtherActor, Hit.ImpactPoint, Hit.ImpactNormal);
	Destroy();
}

void ASB_Projectile::SpawnImpactFX_Multicast_Implementation(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (WeaponModuleData)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			WeaponModuleData->ImpactParticle,
			ImpactPoint,
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal),
			FVector(WeaponModuleData->ImpactParticleScale),
			true,
			EPSCPoolMethod::None,
			true
		);
	}
}

void ASB_Projectile::Debug(AActor* Actor)
{
	FString RoleString = "None";
	FColor Color = FColor::White;

	if (GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy // ";
		Color = FColor::Orange;
	}

	const FString StringToPrint = RoleString + "ASB_Projectile - Projectile destroyed on : " + Actor->GetName();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, Color, StringToPrint);
}