#include "SB_ShipStart.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASB_ShipStart::ASB_ShipStart()
{
	CapsuleCT = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCT"));
	CapsuleCT->SetCollisionProfileName("OverlapOnlyPawn");
	CapsuleCT->InitCapsuleSize(7000.f, 7000.0f);
	RootComponent = CapsuleCT;

	ArrowCT = CreateDefaultSubobject<UArrowComponent>(FName("ArrowCT"));
	ArrowCT->SetupAttachment(RootComponent);

	StandParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("StandParticleCT"));
	StandParticleCT->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	//

	bIsEnabled = true;
	bIsAvailable = true;
}

void ASB_ShipStart::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (TeamID == 1)
		ArrowCT->SetArrowColor(FLinearColor::Blue);
	if (TeamID == 2)
		ArrowCT->SetArrowColor(FLinearColor::Red);
}

void ASB_ShipStart::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		CapsuleCT->OnComponentBeginOverlap.AddDynamic(this, &ASB_ShipStart::OnBeginOverlap);
		CapsuleCT->OnComponentEndOverlap.AddDynamic(this, &ASB_ShipStart::OnEndOverlap);
	}
}

FTransform ASB_ShipStart::GetStartTransform()
{
	return CapsuleCT->GetComponentTransform();
}

void ASB_ShipStart::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedActors.Add(OtherActor);

	bIsAvailable = false;
	UpdateAvailability_Multicast(false);
}

void ASB_ShipStart::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedActors.Remove(OtherActor);

	if (OverlappedActors.Num() == 0)
	{
		bIsAvailable = true;
		UpdateAvailability_Multicast(true);
	}
	else
	{
		bIsAvailable = false;
		UpdateAvailability_Multicast(false);
	}
}

void ASB_ShipStart::UpdateAvailability_Multicast_Implementation(bool bNewIsAvailable)
{
	UpdateAvailability_BPI(bNewIsAvailable);
}


