#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SB_Vehicle.generated.h"

#define DEFAULTCAPSULESIZE 7000.0f
#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, 0.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)

class USB_GameInstance;
class ASB_BattleGameMode;
class ASB_BattlePlayerController;
class ASB_PlayerState;
class USB_ShipMovementComponent;
class USB_ModuleSlotComponent;
class USB_BaseModule;
class USB_WeaponModule;
class USB_ThrusterModule;
class USB_ShieldModule;
class USB_HUDVehicleOTMWidget;
class USB_TargetPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVehicleDurabilityUpdatedDelegate, float, NewDurability, float, MaxDurability);

UCLASS()
class ASB_Vehicle : public ACharacter
{
	GENERATED_BODY()

public:

	ASB_Vehicle(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	
	//

	FORCEINLINE UFUNCTION() const FSB_VehicleData* const GetVehicleData() const { return VehicleData; }
	FORCEINLINE UFUNCTION() USB_ShipMovementComponent* const GetShipMovement() const { return ShipMovement; }
	//FORCEINLINE UFUNCTION() USB_VehicleCameraManager* const GetShipCameraManager() const { return ShipCameraManager; }

private:
	
	USB_GameInstance* GInstance;
	ASB_BattleGameMode* BattleGMode;
	ASB_PlayerState* PState;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName VehicleDataRowName;

	const FSB_VehicleData* VehicleData;

	//
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipMovementComponent* ShipMovement;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//USB_VehicleCameraManager* ShipCameraManager;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Modules

public:
	
	UFUNCTION()
	void LoadConfig(const TArray<FName>& NewConfig, bool bSpawnEmptyModules);

	UFUNCTION()
	void SaveConfig();
	
	//
	
	FORCEINLINE UFUNCTION() TInlineComponentArray<USB_ModuleSlotComponent*> GetModuleSlots() const { return ModuleSlots; }

private:
	
	TInlineComponentArray<USB_ModuleSlotComponent*> ModuleSlots;

	UPROPERTY()
	TArray<USB_BaseModule*> ModuleArray;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Weapons

public:
	
	UFUNCTION()
	void SetPriorityTarget(ASB_Vehicle* NewPriorityTarget);

	//

	FORCEINLINE UFUNCTION() TArray<USB_WeaponModule*> GetWeapons() const { return Weapons; }
	FORCEINLINE UFUNCTION() TArray<USB_TargetPoint*> GetTargetPoints() const { return TargetPoints; }

private:

	UFUNCTION()
	void UpdateWeaponsTargets();

	//
	
	UPROPERTY()
	TArray<USB_WeaponModule*> Weapons;

	UPROPERTY()
	TArray<USB_TargetPoint*> TargetPoints;

	UPROPERTY()
	ASB_Vehicle* PriorityTarget;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Combat

public:
	
	UFUNCTION()
	void ApplyShipDamage(
		float ShipDamage,
		const FVector& HitLocation,
		AController* const InstigatorController,
		float FromModuleDamage
	);
	
	UFUNCTION()
	void UpdateState(ESB_ShipState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateState_Multicast(ESB_ShipState NewState);

	//
	
	FORCEINLINE UFUNCTION() float GetDurability() const { return Durability; }
	FORCEINLINE UFUNCTION() ESB_ShipState GetState() const { return State; }

	//

	FVehicleDurabilityUpdatedDelegate OnVehicleDurabilityUpdated;
	//FDestroyedDelegate OnDestroyed;

private:

	UFUNCTION()
	void OnModuleDamaged(
		USB_BaseModule* const ModuleRef,
		float Damage,
		const FVector& HitLocation,
		AController* const InstigatorController
	);
	
	UFUNCTION()
	void OnRep_Durability();

	//
	
	UPROPERTY(ReplicatedUsing = OnRep_Durability)
	float Durability;

	UPROPERTY()
	ESB_ShipState State;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// View

public:
	
	UFUNCTION()
	void UpdateOwnerViewData(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor);

	UFUNCTION(Server, Reliable)
	void UpdateOwnerViewData_Server(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor);

	//
	
	FORCEINLINE UFUNCTION() const FVector& GetOwnerViewLocation() const { return OwnerViewLocation; }

private:

	UPROPERTY(Replicated)
	FVector OwnerViewLocation;

	UPROPERTY()
	class AActor* OwnerViewActor;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Misc

public:

	UFUNCTION()
	void ToggleOutline(bool bNewIsVisible, int32 StencilValue = 0);

	//
	
	FORCEINLINE UFUNCTION() USB_HUDVehicleOTMWidget* const GetOTMWidget() const { return OTMWidget; }

private:
	
	UFUNCTION()
	void Debug(float DeltaTime);

	//

	UPROPERTY()
	USB_HUDVehicleOTMWidget* OTMWidget;
};
