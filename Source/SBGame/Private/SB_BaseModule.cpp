#include "SB_BaseModule.h"
#include "SB_Ship.h"
#include "SB_GameInstance.h"
#include "SB_DataManager.h"
//
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

USB_BaseModule::USB_BaseModule()
{
	UPrimitiveComponent::SetCollisionProfileName("CharacterMesh");
	SetCustomDepthStencilValue(1);
	SetGenerateOverlapEvents(true);
	//SetIsReplicatedByDefault(true);
}

void USB_BaseModule::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (GetWorld()->IsGameWorld() == false)
		return;

	OwnerShip = Cast<ASB_Ship>(GetOwner());
	DataManager = OwnerShip->GetDataManager();
	BaseModuleData = DataManager->GetBaseModuleDataFromRow(ModuleName);
	if (BaseModuleData)
	{
		SetSkeletalMesh(BaseModuleData->SkeletalMesh);
		SetAnimInstanceClass(BaseModuleData->AnimInstance);
		Durability = BaseModuleData->MaxDurability;
	}
}

void USB_BaseModule::BeginPlay()
{
	Super::BeginPlay();
	
	if (OwnerShip->GetLocalRole() == ROLE_Authority)
	{
		//GetWorld()->GetTimerManager().SetTimer(RepairTimer, this, &USB_BaseModule::RepairOnce, 0.5f, true, 5.0f);
	}
}

void USB_BaseModule::ApplyDamage(float Damage)
{
	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		return;

	if (Durability > 0)
	{
		if (Durability - Damage <= 0)
		{
			Durability = 0;
			OnModuleDestroyed();
		}
		else
		{
			Durability -= Damage;
		}
	}

	ModuleDurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_BaseModule::RepairOnce()
{
	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		return;

	if (Durability + BaseModuleData->RepairAmount >= BaseModuleData->MaxDurability)
	{
		Durability = BaseModuleData->MaxDurability;
	}
	else
	{
		Durability += BaseModuleData->RepairAmount;
	}

	ModuleDurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_BaseModule::OnRep_Durability()
{
	ModuleDurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_BaseModule::OnModuleDestroyed()
{
}

void USB_BaseModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_BaseModule, Durability);
}