#include "Vehicle/SB_ShipMovementComponent.h"
#include "Vehicle/SB_Vehicle.h"
#include "Module/SB_ThrusterModule.h"
#include "RZ_UtilityLibrary.h"
//
#include "EngineUtils.h"
#include "SB_GameState.h"
#include "Net/UnrealNetwork.h"

USB_ShipMovementComponent::USB_ShipMovementComponent()
{
	GroundFriction = 0.1f;

	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ShipMovementComponent::InitializeComponent()
{
	if (GetWorld()->IsGameWorld() == false)
		return;

	SetIsReplicated(true);

	GState = GetWorld()->GetGameState<ASB_GameState>();
}

void USB_ShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningShip = Cast<ASB_Vehicle>(GetOwner());
	if (OwningShip.IsValid())
	{
		TargetRotationYaw = OwningShip->GetActorRotation().Yaw;
		MaxWalkSpeed = OwningShip->GetVehicleData()->MaxMoveSpeed;
		MaxAcceleration = OwningShip->GetVehicleData()->MaxAcceleration;
		GroundFriction = OwningShip->GetVehicleData()->MoveInertia;
		//OwningShip->GetReplicatedMovement()->
	}

	OwningPlayerController = Cast<APlayerController>(OwningShip->GetOwner());
	if (OwningPlayerController.IsValid())
	{
		//OwningPlayerController->InputYawScale = DataManager->ShipSettings.TurnSpeed;
	}
}

void USB_ShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DeltaLol = DeltaTime;
	
	TickMovement(DeltaTime);

	Debug(DeltaTime);
}

void USB_ShipMovementComponent::MoveForward(float AxisValue)
{
	if (OwningShip == nullptr)
		return;

	if (OwningShip->GetState() == ESB_ShipState::Destroyed)
		return;

	if (GState->GameType == ESB_GameType::Campaign || GState->GamePhase != ESB_GamePhase::Playing)
		return;
	
	if (GetOwnerRole() < ROLE_Authority)
		MoveForward_Server(AxisValue);

	if (AxisValue > 0 /*&& OwningShip->ThrusterModule_Back->GetState() == ESB_ModuleState::Ready*/)
	{
		const FRotator Rotation = OwningShip->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		OwningShip->AddMovementInput(Direction);
		ForwardAxisValue = AxisValue;
	}
	else if (AxisValue < 0 /*&& OwningShip->ThrusterModule_Front->GetState() == ESB_ModuleState::Ready*/)
	{
		const FRotator Rotation = OwningShip->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * -1;
		OwningShip->AddMovementInput(Direction);
		ForwardAxisValue = AxisValue;
	}
	else
	{
		ForwardAxisValue = 0.0f;
	}
}

void USB_ShipMovementComponent::MoveForward_Server_Implementation(float AxisValue)
{
	MoveForward(AxisValue);
}

void USB_ShipMovementComponent::TurnRight(float AxisValue)
{
	if (OwningShip == nullptr)
		return;

	if (OwningShip->GetState() == ESB_ShipState::Destroyed)
		return;

	if (OwningShip->GetLocalRole() < ROLE_Authority)
	{
		TurnRight_Server(AxisValue);
	}

	if (AxisValue > 0 /*&& OwningShip->ThrusterModule_Right->GetState() == ESB_ModuleState::Ready)*/ ||
		AxisValue < 0 /*&& OwningShip->ThrusterModule_Left->GetState() == ESB_ModuleState::Ready)*/)
	{
		RightAxisValue = AxisValue;

		if (OwningPlayerController.IsValid())
		{
			OwningPlayerController->AddYawInput(AxisValue * OwningShip->GetVehicleData()->TurnRate * GetWorld()->GetDeltaSeconds());
			//TargetRotationYaw = TargetRotationYaw + AxisValue * DataManager->ShipSettings.TurnSpeed;// * DeltaLol;//GetWorld()->GetDeltaSeconds();
		}
	}
	else
	{
		RightAxisValue = 0.0f;
	}
}

void USB_ShipMovementComponent::TurnRight_Server_Implementation(float AxisValue)
{
	TurnRight(AxisValue);
}

void USB_ShipMovementComponent::TickMovement(float DeltaTime)
{
	if (OwningShip == nullptr)
		return;

	if (OwningShip->GetState() == ESB_ShipState::Destroyed)
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
	if (OwningShip == nullptr)
		return;

	FString RoleString = "None";
	FColor Color = FColor::White;

	if (OwningShip->GetLocalRole() == ROLE_Authority)
	{
		RoleString = "Authority // ";
		Color = FColor::Cyan;
	}
	if (OwningShip->GetLocalRole() == ROLE_AutonomousProxy)
	{
		RoleString = "AutonomousProxy // ";
		Color = FColor::Yellow;
	}
	if (OwningShip->GetLocalRole() == ROLE_SimulatedProxy)
	{
		RoleString = "SimulatedProxy // ";
		Color = FColor::Orange;
	}

	//const FString DebugString = RoleString + this->GetName() + " // ForwardInput : " + FString::FromInt(ForwardInput) + " // RightInput : " + FString::FromInt(RightInput);

	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, DebugString);
}

#pragma endregion