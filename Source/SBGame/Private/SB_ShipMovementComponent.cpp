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
	MaxForwardInput = 3;
	MinForwardInput = -1;
	MaxRightInput = 1;
	ForwardInput = 0;
	RightInput = 0;

	//

	PrimaryComponentTick.bCanEverTick = true;
}

void USB_ShipMovementComponent::InitializeComponent()
{
	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}
}

void USB_ShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerShip = Cast<ASB_Ship>(GetOwner());
	RotationYaw = OwnerShip->GetActorRotation().Yaw;
}

void USB_ShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickMovement(DeltaTime);

	if (OwnerShip)
	{
		if (OwnerShip->GetLocalRole() == ROLE_Authority)
		{
			//Cast<APlayerController>(OwnerShip->GetOwner())->SetControlRotation(FRotator(0.0f, RotationYaw, 0.0f));
		}
		//OwnerShip->SetActorRotation(FRotator(OwnerShip->GetActorRotation().Pitch, RotationYaw, OwnerShip->GetActorRotation().Roll));
	}

	Debug(DeltaTime);
}

void USB_ShipMovementComponent::AddForwardInput(int32 ForwardInputToAdd)
{
	if (OwnerShip == nullptr)
		return;

	if (OwnerShip->GetState() == ESB_ShipState::Destroyed)
		return;

	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		AddForwardInput_Server(ForwardInputToAdd);

	if (ForwardInputToAdd > 0)
	{
		if (ForwardInput + ForwardInputToAdd <= MaxForwardInput)
		{
			ForwardInput += ForwardInputToAdd; 
		}
	}

	if (ForwardInputToAdd < 0)
	{
		if (ForwardInput + ForwardInputToAdd >= MinForwardInput)
		{
			ForwardInput += ForwardInputToAdd;
		}
	}

	ShipMovementUpdatedEvent.Broadcast(ForwardInput, RightInput);
}

void USB_ShipMovementComponent::AddForwardInput_Server_Implementation(int32 ForwardInputToAdd)
{
	AddForwardInput(ForwardInputToAdd);
}

void USB_ShipMovementComponent::AddRightInput(int32 RightInputToAdd)
{
	if (OwnerShip == nullptr)
		return;

	if (OwnerShip->GetState() == ESB_ShipState::Destroyed)
		return;

	if (OwnerShip->GetLocalRole() < ROLE_Authority)
		AddRightInput_Server(RightInputToAdd);

	if (RightInputToAdd > 0)
	{
		if (RightInput + RightInputToAdd <= MaxRightInput)
		{
			RightInput += RightInputToAdd;
		}
	}

	if (RightInputToAdd < 0)
	{
		if (RightInput + RightInputToAdd >= MaxRightInput * -1)
		{
			RightInput += RightInputToAdd;
		}
	}

	ShipMovementUpdatedEvent.Broadcast(ForwardInput, RightInput);
}

void USB_ShipMovementComponent::AddRightInput_Server_Implementation(int32 RightInputToAdd)
{
	AddRightInput(RightInputToAdd);
}

void USB_ShipMovementComponent::TickMovement(float DeltaTime)
{
	if (OwnerShip == nullptr)
		return;

	if (OwnerShip->GetState() == ESB_ShipState::Destroyed)
		return;

	if (ForwardInput != 0)
	{
		if (ForwardInput > 0)
		{
			MaxWalkSpeed = DataManager->ShipSettings.BaseMoveSpeed * ForwardInput;
			const FRotator Rotation = OwnerShip->GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			OwnerShip->AddMovementInput(Direction);
		}
		else
		{
			MaxWalkSpeed = DataManager->ShipSettings.BaseMoveSpeed * ForwardInput * -1;
			const FRotator Rotation = OwnerShip->GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * -1;
			OwnerShip->AddMovementInput(Direction);
		}
	}

	if (RightInput != 0)
	{
		if (RightInput > 0)
		{
			RotationYaw += DataManager->ShipSettings.BaseTurnSpeed * DeltaTime;
			//OwnerShip->AddActorWorldRotation(FRotator(0.0f, DataManager->ShipSettings.BaseTurnSpeed * DeltaTime, 0.0f));
		}
		else
		{
			RotationYaw -= DataManager->ShipSettings.BaseTurnSpeed * DeltaTime;

			//OwnerShip->AddActorWorldRotation(FRotator(0.0f, DataManager->ShipSettings.BaseTurnSpeed * DeltaTime * -1, 0.0f));
		}
	}
}

#pragma region +++++ Network / Debug ...

void USB_ShipMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_ShipMovementComponent, RotationYaw);
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

	const FString DebugString = RoleString + this->GetName() + " // ForwardInput : " + FString::FromInt(ForwardInput) + " // RightInput : " + FString::FromInt(RightInput);

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, DebugString);
}

#pragma endregion