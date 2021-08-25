#include "SB_PlayerStart.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASB_PlayerStart::ASB_PlayerStart()
{
	CapsuleCT = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCT"));
	CapsuleCT->SetCollisionProfileName("OverlapOnlyPawn");
	CapsuleCT->InitCapsuleSize(15000.f, 15000.0f);
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

void ASB_PlayerStart::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (TeamID == 0)
		ArrowCT->SetArrowColor(FLinearColor::White);
	if (TeamID == 1)
		ArrowCT->SetArrowColor(FLinearColor::Blue);
	if (TeamID == 2)
		ArrowCT->SetArrowColor(FLinearColor::Red);
}

void ASB_PlayerStart::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		CapsuleCT->OnComponentBeginOverlap.AddDynamic(this, &ASB_PlayerStart::OnBeginOverlap);
		CapsuleCT->OnComponentEndOverlap.AddDynamic(this, &ASB_PlayerStart::OnEndOverlap);
	}
}

FTransform ASB_PlayerStart::GetStartTransform()
{
	return CapsuleCT->GetComponentTransform();
}

void ASB_PlayerStart::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedActors.Add(OtherActor);

	bIsAvailable = false;
	UpdateAvailability_Multicast(false);
}

void ASB_PlayerStart::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void ASB_PlayerStart::UpdateAvailability_Multicast_Implementation(bool bNewIsAvailable)
{
	UpdateAvailability_BPI(bNewIsAvailable);
}


