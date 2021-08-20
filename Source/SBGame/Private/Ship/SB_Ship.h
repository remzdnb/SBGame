#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SB_Ship.generated.h"

#define DEFAULTCAPSULESIZE 7000.0f
#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, 0.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)
#define MAXAUTOLOCKCOMPONENTS 4

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDurabilityUpdatedDelegate, float, NewDurability, float, MaxDurability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyedDelegate, const APlayerState* const, Instigator);

class USB_GameInstance;
class ASB_BattleGameMode;
class ASB_PlayerState;
class USB_ShipCameraManager;
class USB_ShipMovementComponent;
class USB_ModuleSlotComponent;
class USB_BaseModule;
class USB_WeaponModule;
class USB_ThrusterModule;
class USB_ShieldModule;
class USB_HUDVehicleOTMWidget;
class USB_TargetPoint;

UCLASS()
class ASB_Ship : public ACharacter
{
	GENERATED_BODY()

public:

	ASB_Ship(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	
	//

	FORCEINLINE UFUNCTION() const FSB_ShipData* const GetShipData() const { return ShipData; }
	FORCEINLINE UFUNCTION() USB_ShipMovementComponent* const GetShipMovement() const { return ShipMovement; }
	//FORCEINLINE UFUNCTION() USB_ShipCameraManager* const GetShipCameraManager() const { return ShipCameraManager; }

private:
	
	USB_GameInstance* GInstance;
	ASB_BattleGameMode* BattleGMode;
	ASB_PlayerState* PState;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ShipDataRowName;

	const FSB_ShipData* ShipData;

	//
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipMovementComponent* ShipMovement;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//USB_ShipCameraManager* ShipCameraManager;
	
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Weapons

public:
	
	UFUNCTION()
	void SelectPriorityTarget(ASB_Ship* NewTargetShip);

	//

	FORCEINLINE UFUNCTION() TArray<USB_WeaponModule*> GetWeapons() const { return Weapons; }
	FORCEINLINE UFUNCTION() TArray<USB_TargetPoint*> GetTargetPoints() const { return TargetPoints; }

private:

	UFUNCTION()
	void UpdateSecondaryTargets();

	//
	
	UPROPERTY()
	TArray<USB_WeaponModule*> Weapons;

	UPROPERTY()
	TArray<USB_TargetPoint*> TargetPoints;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Combat

public:
	
	UFUNCTION()
	void ApplyShipDamage(float ShipDamage, const FVector& HitLocation, AController* const InstigatorController, float FromModuleDamage);
	
	UFUNCTION()
	void UpdateState(ESB_ShipState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateState_Multicast(ESB_ShipState NewState);

	//
	
	FORCEINLINE UFUNCTION() float GetDurability() const { return Durability; }
	FORCEINLINE UFUNCTION() ESB_ShipState GetState() const { return State; }

	//

	FDurabilityUpdatedDelegate OnDurabilityUpdated;
	FDestroyedDelegate OnDestroyed;

private:

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
