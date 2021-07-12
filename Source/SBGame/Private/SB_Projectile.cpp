#include "SB_Projectile.h"
#include "SB_Ship.h"
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
	CollisionSphereCT = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphereCT"));
	CollisionSphereCT->InitSphereRadius(1.0f);
	CollisionSphereCT->SetCollisionProfileName("Projectile");
	CollisionSphereCT->bTraceComplexOnMove = true;
	CollisionSphereCT->SetGenerateOverlapEvents(true);
	CollisionSphereCT->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = CollisionSphereCT;

	MeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshCT"));
	MeshCT->SetCollisionProfileName("IgnoreAll");
	MeshCT->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	MeshCT->SetupAttachment(RootComponent);

	ParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleCT"));
	ParticleCT->SetupAttachment(RootComponent);

	ProjectileMovementCT = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementCT"));
	ProjectileMovementCT->ProjectileGravityScale = 0.0f;
	ProjectileMovementCT->bRotationFollowsVelocity = true;

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ASB_Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	SetLifeSpan(5.0f);

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionSphereCT->OnComponentBeginOverlap.AddDynamic(this, &ASB_Projectile::OnOverlap);
	}
}

void ASB_Projectile::BeginPlay()
{
	Super::BeginPlay();

	OwnerShip = Cast<ASB_Ship>(GetOwner());
	if (OwnerShip)
	{
		ProjectileData = OwnerShip->GetDataManager()->GetProjectileDataFromRow(DataRowName);
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

void ASB_Projectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileData->Damage, FVector::ZeroVector, SweepResult, Cast<AController>(OwnerShip->GetOwner()), nullptr, nullptr); // ToDo : is ship always owner, even if not possessed ?
		SpawnImpactFX_Multicast(OtherActor, SweepResult.ImpactPoint, SweepResult.ImpactNormal);
		//Debug(OtherActor);
		Destroy();
	}
}

void ASB_Projectile::SpawnImpactFX_Multicast_Implementation(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (ProjectileData == nullptr) // crash if it doesnt have time to initialize ?
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileData->ImpactParticle, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(ImpactNormal), FVector(ProjectileData->ImpactParticleScale), true, EPSCPoolMethod::None, true);

	if (OwnerShip->GetDataManager()->GameSettings.bIsDebugEnabled_Projectile == false)
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

	const FString StringToPrint = RoleString + "ASB_Projectile - Projectile destroyed on : " + HitActor->GetName();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, Color, StringToPrint);
}

void ASB_Projectile::Debug(AActor* Actor)
{
	if (OwnerShip->GetDataManager()->GameSettings.bIsDebugEnabled_Projectile == false)
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