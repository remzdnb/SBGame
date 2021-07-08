#include "SB_Shield.h"
#include "SB_DataManager.h"
//
#include "Components/StaticMeshComponent.h"

ASB_Shield::ASB_Shield()
{
	SceneCT = CreateDefaultSubobject<USceneComponent>(FName("SceneCT"));
	RootComponent = SceneCT;

	MeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshCT"));
	MeshCT->SetupAttachment(SceneCT);
	MeshCT->SetCollisionProfileName("IgnoreAll");
	MeshCT->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	//MeshCT->SetRelativeLocation(FVector(DEFAULTCAPSULESIZE * -1, 0.0f, 0.0f));
	MeshCT->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//MeshCT->SetWorldScale3D(FVector(DEFAULTCAPSULESIZE / 40.0f));

	PrimaryActorTick.bCanEverTick = false;
}

void ASB_Shield::Init(const class ASB_DataManager* const NewDataManager)
{
	DataManager = NewDataManager;
}

void ASB_Shield::BeginPlay()
{
	Super::BeginPlay();

	MeshCT->SetMaterial(0, DataManager->ShieldSettings.SetupMaterial);
	SetActorHiddenInGame(true);
}

void ASB_Shield::OnSetupStart()
{
	//MeshCT->SetMaterial(0, DataManager->ShieldSettings.SetupMaterial);
	SetActorHiddenInGame(false);
}

void ASB_Shield::OnSetupStop()
{
	SetActorHiddenInGame(true);
}

void ASB_Shield::OnDeployedBPN_Implementation()
{
	MeshCT->SetMaterial(0, DataManager->ShieldSettings.DeployedMaterial);
	MeshCT->SetCollisionProfileName("CharacterMesh");
}

void ASB_Shield::OnDestroyedBPN_Implementation()
{
	MeshCT->SetCollisionProfileName("IgnoreAll");
}
