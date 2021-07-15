#include "SB_Projectile.h"
#include "SB_Ship.h"
#include "SB_BaseModule.h"
#include "SB_DataManager.h"
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
	VisualMesh->SetCollisionProfileName("IgnoreAll");
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

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionSphere->OnComponentHit.AddDynamic(this, &ASB_Projectile::OnHit);
	}
	
	SetLifeSpan(5.0f);
}

void ASB_Projectile::BeginPlay()
{
	Super::BeginPlay();

	OwningShip = Cast<ASB_Ship>(GetOwner());
	if (OwningShip.IsValid())
	{
		OwningController = Cast<AController>(OwningShip->GetOwner());
		CollisionSphere->IgnoreActorWhenMoving(OwningShip.Get(), true);
		ProjectileData = OwningShip->GetDataManager()->GetProjectileDataFromRow(DataRowName);
		if (ProjectileData == nullptr)
		{
			Destroy();
		}
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
		//UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileData->Damage, FVector::ZeroVector, Hit, Cast<AController>(OwnerShip->GetOwner()), nullptr, nullptr); // ToDo : is ship always owner, even if not possessed ?
		USB_BaseModule* DamagedModule = Cast<USB_BaseModule>(OtherComp);
		if (DamagedModule)
		{
			DamagedModule->ApplyDamage(ProjectileData->Damage, Hit.Location, OwningController.Get());
		}

		SpawnImpactFX_Multicast(OtherActor, Hit.ImpactPoint, Hit.ImpactNormal);
		//Debug(OtherActor);
		const FString StringToPrint = "ASB_Projectile HIT - Projectile destroyed on Actor : " + OtherActor->GetName() + "// Component : " + OtherComp->GetName();

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, StringToPrint);
		Destroy();
	}
}

void ASB_Projectile::SpawnImpactFX_Multicast_Implementation(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (ProjectileData == nullptr) // crash if it doesnt have time to initialize ?
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileData->ImpactParticle, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(ImpactNormal), FVector(ProjectileData->ImpactParticleScale), true, EPSCPoolMethod::None, true);

	if (OwningShip->GetDataManager()->GameSettings.bIsDebugEnabled_Projectile == false)
		return;

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
}

void ASB_Projectile::Debug(AActor* Actor)
{
	if (OwningShip->GetDataManager()->GameSettings.bIsDebugEnabled_Projectile == false)
		return;

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