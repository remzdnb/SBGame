#include "Module/Weapon/SB_Beam.h"
//
#include "Particles/ParticleSystemComponent.h"

ASB_Beam::ASB_Beam()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = RootScene;

	BeamParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("BeamParticle"));
	BeamParticle->SetupAttachment(RootScene);
	
	PrimaryActorTick.bCanEverTick = true;
}

void ASB_Beam::BeginPlay()
{
	Super::BeginPlay();
}

void ASB_Beam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

