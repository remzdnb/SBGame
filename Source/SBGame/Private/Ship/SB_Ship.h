#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SB_Ship.generated.h"

#define DEFAULTCAPSULESIZE 7000.0f
#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, 0.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)
#define MAXAUTOLOCKCOMPONENTS 4

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedWeaponUpdated, uint8, NewSelectedWeaponID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDurabilityUpdatedDelegate, float, NewDurability, float, MaxDurability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyedDelegate, const APlayerState* const, Instigator);

class USB_ShipCameraManager;
class USB_ShipMovementComponent;
class USB_ShieldModule;
class USB_ShipOTMWidget;

UCLASS()
class ASB_Ship : public ACharacter
{
	GENERATED_BODY()

public:

	ASB_Ship(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_PlayerState() override;
	//

	FORCEINLINE UFUNCTION() USB_ShipMovementComponent* const GetShipMovement() const { return ShipMovement; }
	FORCEINLINE UFUNCTION() USB_ShipCameraManager* const GetShipCameraManager() const { return ShipCameraManager; }

private:

	class ASB_DataManager* DataManager;
	class USB_GameInstance* GInstance;
	class ASB_PlayerState* PState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipMovementComponent* ShipMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_ShipCameraManager* ShipCameraManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ShipDataRowName;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Modules

public:
	
	UFUNCTION()
	void LoadConfig(const TArray<FName>& NewConfig);

	UFUNCTION()
	void SaveConfig();
	
	//
	
	FORCEINLINE UFUNCTION() TInlineComponentArray<class USB_ModuleSlotComponent*> GetModuleSlots() const { return ModuleSlots; }
	FORCEINLINE UFUNCTION() TInlineComponentArray<class USB_BaseModule*> GetAllModules() const { return BaseModules; }
	FORCEINLINE UFUNCTION() TInlineComponentArray<class USB_BaseWeaponModule*> GetWeaponModules() const { return WeaponModules; }

private:

	TInlineComponentArray<class USB_ModuleSlotComponent*> ModuleSlots;
	TInlineComponentArray<class USB_BaseModule*> BaseModules;
	TInlineComponentArray<class USB_BaseWeaponModule*> WeaponModules;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Weapons

public:

	UFUNCTION()
	void SelectWeapon(uint8 WeaponID, bool bToggleSelection = true, bool bNewIsSelected = true);

	UFUNCTION(Server, Reliable)
	void SelectWeapon_Server(uint8 WeaponID, bool bToggleSelection = true, bool bNewIsSelected = true);

	UFUNCTION()
	void StartFireSelectedWeapons();

	UFUNCTION(Server, Reliable)
	void StartFireSelectedWeapons_Server();

	UFUNCTION()
	void StopFireAllWeapons();

	UFUNCTION(Server, Reliable)
	void StopFireAllWeapons_Server();

	UFUNCTION()
	void StartAutoLockSelectedWeapon();

	UFUNCTION()
	void StopAutoLockSelectedWeapon();

private:

	UPROPERTY()
	uint8 SelectedWeaponID;

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

	FORCEINLINE UFUNCTION() const FRotator& GetOwnerViewRotation() const { return OwnerViewRotation; }
	FORCEINLINE UFUNCTION() const FVector& GetOwnerViewLocation() const { return OwnerViewLocation; }

private:

	UPROPERTY(Replicated)
	FRotator OwnerViewRotation; // ToDo : Delete, useless, did it for shield setup. // Lol

	UPROPERTY(Replicated)
	FVector OwnerViewLocation;

	UPROPERTY()
	class AActor* OwnerViewActor;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Misc

public:

	UFUNCTION()
	void ToggleOutline(bool bNewIsVisible, int32 StencilValue = 0);

private:
	
	UFUNCTION()
	void Debug(float DeltaTime);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	FORCEINLINE UFUNCTION() const ASB_DataManager* const GetDataManager() const { return DataManager; }
	FORCEINLINE UFUNCTION() TArray<USceneComponent*> GetAutoLockCTs() const { return AutoLockCTs; }


	FORCEINLINE UFUNCTION() uint8 GetSelectedWeaponID() const { return SelectedWeaponID; }
	FORCEINLINE UFUNCTION() USB_ShipOTMWidget* const GetOTMWidget() const { return OTMWidget; }
	
	// Delegates

	FSelectedWeaponUpdated SelectedWeaponUpdatedEvent;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class USceneComponent*> AutoLockCTs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* CircleParticleCT;
	//

	UPROPERTY()
	USB_ShipOTMWidget* OTMWidget;

};
