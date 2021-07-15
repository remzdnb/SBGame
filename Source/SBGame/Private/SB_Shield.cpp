#include "SB_Shield.h"
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
	ShieldMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	//MeshCT->SetRelativeLocation(FVector(DEFAULTCAPSULESIZE * -1, 0.0f, 0.0f));
	//MeshCT->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//MeshCT->SetWorldScale3D(FVector(DEFAULTCAPSULESIZE / 40.0f));

	PrimaryActorTick.bCanEverTick = false;
	//bReplicates = true;
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

	ShieldMesh->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
	ShieldMesh->SetRelativeScale3D(DataManager->ShieldSettings.ShieldMeshScale);

	//SetActorHiddenInGame(true);
}

void ASB_Shield::OnDeployedBPN_Implementation()
{
	//MeshCT->SetCollisionProfileName("CharacterMesh");
}

void ASB_Shield::OnDestroyedBPN_Implementation()
{
	ShieldMesh->SetCollisionProfileName("NoCollision");
}

void ASB_Shield::ApplyDamage(const float Damage, const FVector& HitLocation, AController* const InstigatorController)
{
	/*if (Durability - Damage <= 0)
	{
		Durability = 0.0f;
	}
	else
	{
		Durability -= Damage;
	}*/
}
