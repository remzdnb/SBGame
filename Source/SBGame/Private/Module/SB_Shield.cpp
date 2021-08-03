#include "Module/SB_Shield.h"
#include "Module/SB_ShieldModule.h"
#include "Ship/SB_Ship.h"
#include "SB_DataManager.h"
//
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

ASB_Shield::ASB_Shield()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = RootScene;

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootScene);
	ShieldMesh->SetCollisionProfileName("NoCollision");
	ShieldMesh->SetGenerateOverlapEvents(false);
	ShieldMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
}

void ASB_Shield::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	OwningShip = Cast<ASB_Ship>(GetOwner());
	OwningShieldModule = OwningShip->ShieldModule;

	ShieldMesh->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
	ShieldMesh->SetRelativeScale3D(DataManager->ShieldSettings.ShieldMeshScale);
}

void ASB_Shield::OnDeployedBPN_Implementation()
{
	ShieldMesh->SetCollisionProfileName("Shield");
}

void ASB_Shield::OnDestroyedBPN_Implementation()
{
	ShieldMesh->SetCollisionProfileName("NoCollision");
}

void ASB_Shield::ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController)
{
	if (OwningShieldModule.IsValid())
	{
		OwningShieldModule->ApplyShieldDamage(Damage, HitLocation, InstigatorController);
	}
}
