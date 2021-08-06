#include "Module/SB_ModuleSlotActor.h"

ASB_ModuleSlotActor::ASB_ModuleSlotActor()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = RootScene;

	DemoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("DemoMesh"));
	DemoMesh->SetupAttachment(RootScene);
	DemoMesh->SetCollisionProfileName("NoCollision");
	DemoMesh->SetHiddenInGame(true);
	
	PrimaryActorTick.bCanEverTick = false;
}

void ASB_ModuleSlotActor::BeginPlay()
{
	Super::BeginPlay();
}

