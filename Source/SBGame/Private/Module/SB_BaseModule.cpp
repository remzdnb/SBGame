#include "Module/SB_BaseModule.h"
#include "Ship/SB_Ship.h"
#include "SB_GameInstance.h"
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
	bEnablePerPolyCollision = true;
	SetGenerateOverlapEvents(true);
	SetCustomDepthStencilValue(1);
	SetIsReplicatedByDefault(true);

	GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	bIsHovered = false;
	bIsSelected = false;
	State = ESB_ModuleState::Ready;
}

void USB_BaseModule::Init(const FSB_ModuleSlotData& NewModuleSlotData, const FName& NewModuleRowName)
{
	//OwningShip = Cast<ASB_Ship>(GetOuter());
	
	ModuleSlotData = NewModuleSlotData;
	ModuleRowName = NewModuleRowName;

 	BaseModuleData = GInstance->GetBaseModuleDataFromRow(ModuleRowName);
	if (BaseModuleData)
	{
		SetSkeletalMesh(BaseModuleData->SkeletalMesh);
		SetAnimInstanceClass(BaseModuleData->AnimInstance);
		SkeletalMesh->bEnablePerPolyCollision = true;
		Durability = BaseModuleData->MaxDurability;
		SetWorldScale3D(BaseModuleData->WorldScale);
	}
}

void USB_BaseModule::OnHoverStart()
{
	if (BaseModuleData == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("USB_BaseModule::OnHoverStart : BaseModuleData == nullptr"));
		return;
	}
	
	if (bIsSelected == false)
	{
		if (BaseModuleData->bIsSelectable)
		{
			SetCustomDepthStencilValue(2);
		}
		else
		{
			SetCustomDepthStencilValue(1);
		}
		
		SetRenderCustomDepth(true);
		bIsHovered = true;
	}
}

void USB_BaseModule::OnHoverStop()
{
	SetRenderCustomDepth(false);
	bIsHovered = false;
}

void USB_BaseModule::ToggleHighlight(bool bNewIsEnabled)
{
	SetCustomDepthStencilValue(3);
	
	SetRenderCustomDepth(bNewIsEnabled);
	bIsSelected = bNewIsEnabled;
	
	bIsHovered = false;
	
	OnSelectionUpdated.Broadcast(bNewIsEnabled);
}

void USB_BaseModule::ApplyDamageFromProjectile(float Damage, const FVector& HitLocation, AController* const InstigatorController)
{
	// Apply module damage.
	if (Durability > 0)
	{
		if (Durability - Damage <= 0)
		{
			Durability = 0;
			UpdateState(ESB_ModuleState::Repairing);
		}
		else
		{
			Durability -= Damage;
		}
	}

	// Auto start repair.
	if (Durability < BaseModuleData->MaxDurability && RepairTimer.IsValid() == false)
	{
		RepairTimer.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(RepairTimer, this, &USB_BaseModule::RepairOnce, 0.5f, true, 2.0f);
	}

	// Apply ship damage.
	//OwningShip->ApplyShipDamage(Damage * BaseModuleData->ShipDamageModifier, HitLocation, InstigatorController, Damage);

	//
	OnDurabilityUpdated.Broadcast(Durability);
	
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, *("USB_BaseModule::ApplyDamage // New Durability : " + FString::FromInt(Durability))); 
}


void USB_BaseModule::RepairOnce()
{
	if (Durability + BaseModuleData->RepairAmount >= BaseModuleData->MaxDurability)
	{
		Durability = BaseModuleData->MaxDurability;
	}
	else
	{
		Durability += BaseModuleData->RepairAmount;
	}

	if (Durability == BaseModuleData->MaxDurability)
	{
		UpdateState(ESB_ModuleState::Ready);
		RepairTimer.Invalidate();
	}

	OnDurabilityUpdated.Broadcast(Durability);
}

void USB_BaseModule::UpdateState(const ESB_ModuleState NewState)
{
	State = NewState;

	OnRep_State();
}

void USB_BaseModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_BaseModule, State);
	DOREPLIFETIME(USB_BaseModule, Durability);
}

void USB_BaseModule::OnRep_State()
{
	OnStateUpdated.Broadcast(State);
}

void USB_BaseModule::OnRep_Durability()
{
	OnDurabilityUpdated.Broadcast(Durability);
}