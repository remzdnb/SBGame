#include "SB_ShipMovementComponent.h"
#include "SB_Ship.h"
#include "SB_DataManager.h"
#include "SB_UtilityLibrary.h"
//
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

USB_ShipMovementComponent::USB_ShipMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ShipMovementComponent::InitializeComponent()
{
	if (GetWorld()->IsGameWorld() == false)
		return;

	SetIsReplicated(true);
}

void USB_ShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerShip = Cast<ASB_Ship>(GetOwner());
	if (OwnerShip)
	{
		DataManager = OwnerShip->GetDataManager();
		TargetRotationYaw = OwnerShip->GetActorRotation().Yaw;
		MaxWalkSpeed = DataManager->ShipSettings.MoveSpeed;
	}
}

void USB_ShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickMovement(DeltaTime);

	Debug(DeltaTime);
}

void USB_ShipMovementComponent::MoveForward(float AxisValue)
{
	if (DataManager == nullptr || OwnerShip == nullptr)
		return;

	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		MoveForward_Server(AxisValue);

	ForwardAxisValue = AxisValue;

	if (AxisValue > 0)
	{
		const FRotator Rotation = OwnerShip->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		OwnerShip->AddMovementInput(Direction);
	}
	else if (AxisValue < 0)
	{
		const FRotator Rotation = OwnerShip->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * -1;
		OwnerShip->AddMovementInput(Direction);
	}

}

void USB_ShipMovementComponent::MoveForward_Server_Implementation(float AxisValue)
{
	MoveForward(AxisValue);
}

void USB_ShipMovementComponent::TurnRight(float AxisValue)
{
	if (GetWorld() == nullptr || DataManager == nullptr || OwnerShip == nullptr)
		return;

	if (OwnerShip->GetLocalRole() < ROLE_Authority)
	{
		TurnRight_Server(AxisValue);
		return;
	}

	RightAxisValue = AxisValue;
	TargetRotationYaw = TargetRotationYaw + AxisValue * DataManager->ShipSettings.TurnSpeed * GetWorld()->GetDeltaSeconds();
}

void USB_ShipMovementComponent::TurnRight_Server_Implementation(float AxisValue)
{
	TurnRight(AxisValue);
}

void USB_ShipMovementComponent::TickMovement(float DeltaTime)
{
	if (OwnerShip == nullptr)
		return;

	if (OwnerShip->GetState() == ESB_ShipState::Destroyed)
		return;

}

#pragma region +++++ Network / Debug ...

void USB_ShipMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_ShipMovementComponent, ForwardAxisValue);
	DOREPLIFETIME(USB_ShipMovementComponent, RightAxisValue);
	DOREPLIFETIME(USB_ShipMovementComponent, TargetRotationYaw);
}

void USB_ShipMovementComponent::Debug(float DeltaTime)
{
	if (OwnerShip == nullptr || DataManager->GameSettings.bIsDebugEnabled_ShipMovement == false)
		return;

	FString RoleString = "None";
	FColor Color = FColor::White;

	if (OwnerShip->GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (OwnerShip->GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (OwnerShip->GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy // ";
		Color = FColor::Orange;
	}

	//const FString DebugString = RoleString + this->GetName() + " // ForwardInput : " + FString::FromInt(ForwardInput) + " // RightInput : " + FString::FromInt(RightInput);

	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, DebugString);
}

#pragma endregion