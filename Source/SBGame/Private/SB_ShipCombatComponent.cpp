#include "SB_ShipCombatComponent.h"
#include "SB_Ship.h"
#include "SB_BaseModule.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
//
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

USB_ShipCombatComponent::USB_ShipCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
	SetIsReplicatedByDefault(true);
}

void USB_ShipCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerShip = (ASB_Ship*)(GetOwner());
	if (OwnerShip)
	{
		DataManager = OwnerShip->GetDataManager();
		Durability = DataManager->ShipSettings.MaxDurability;
		OwnerShip->OnTakePointDamage.AddDynamic(this, &USB_ShipCombatComponent::TakePointDamage);
	}
}

void USB_ShipCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USB_ShipCombatComponent::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (Durability == 0)
		return;

	USB_BaseModule* const BaseModule = Cast<USB_BaseModule>(FHitComponent);
	if (BaseModule)
	{
		BaseModule->ApplyDamage(Damage);
	}

	if (Durability - Damage <= 0)
	{
		Durability = 0.0f;
		OwnerShip->UpdateState(ESB_ShipState::Destroyed);
	}
	else
	{
		Durability -= Damage;
	}

	ASB_PlayerController* const InstigatorController = Cast<ASB_PlayerController>(InstigatedBy);
	if (InstigatorController)
	{
		InstigatorController->OnDamageDealt(HitLocation, Damage);
	}

	OnRep_Durability();

	//

	FString StringToPrint = this->GetName() + "::TakePointDamage // Instigator : " + InstigatedBy->GetName() + " / Causer : " /* + DamageCauser->GetName() +*/ " / Amount : " + FString::SanitizeFloat(Damage);
	UE_LOG(LogTemp, Display, TEXT("%s"), *StringToPrint);
}

void USB_ShipCombatComponent::OnRep_Durability()
{
	DurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_ShipCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_ShipCombatComponent, Durability);
}