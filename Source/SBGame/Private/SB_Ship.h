///// SB_Ship.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDurabilityUpdatedDelegate, float, NewDurability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyedDelegate, const APlayerState* const, Instigator);

class ASB_DataManager;
class USB_BaseModule;
class USB_WeaponModule;
class USB_ShieldModule;
class USB_ShipOTMWidget;

UCLASS()
class ASB_Ship : public ACharacter
{
	GENERATED_BODY()

public:

	ASB_Ship(const FObjectInitializer& ObjectInitializer);

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// View

	UFUNCTION()
	void UpdateOwnerViewData(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor);

	UFUNCTION(Server, Reliable)
	void UpdateOwnerViewData_Server(const FVector& NewOwnerViewLocation, AActor* NewOwnerViewActor);

	// Weapons

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

	// Combat

	UFUNCTION()
	void ApplyDamage(const float Damage, const FVector& HitLocation, AController* const InstigatorController);
	
	UFUNCTION()
	void UpdateState(ESB_ShipState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateState_Multicast(ESB_ShipState NewState);

	// Cosmetic

	UFUNCTION()
	void ToggleOutline(bool bNewIsVisible);

	// Getters

	FORCEINLINE UFUNCTION() const class ASB_DataManager* const GetDataManager() const { return DataManager; }

	FORCEINLINE UFUNCTION() class USB_ShipMovementComponent* const GetShipMovementCT() const { return MovementCT; }
	FORCEINLINE UFUNCTION() class USB_ShipCameraManager* const GetShipCameraManager() const { return ShipCameraManager; }

	FORCEINLINE UFUNCTION() TArray<class USB_ThrusterModule*> GetThrusterModules() const { return ThrusterModules; }
	FORCEINLINE UFUNCTION() TInlineComponentArray<USB_WeaponModule*> GetWeaponModules() const { return WeaponModules; }
	FORCEINLINE UFUNCTION() class USB_ShieldModule* const GetShieldModule() const { return ShieldModule; }
	FORCEINLINE UFUNCTION() TArray<class USceneComponent*> GetAutoLockCTs() const { return AutoLockCTs; }

	FORCEINLINE UFUNCTION() ESB_ShipState GetState() const { return State; }
	FORCEINLINE UFUNCTION() const FRotator& GetOwnerViewRotation() const { return OwnerViewRotation; }
	FORCEINLINE UFUNCTION() const FVector& GetOwnerViewLocation() const { return OwnerViewLocation; }
	FORCEINLINE UFUNCTION() uint8 GetSelectedWeaponID() const { return SelectedWeaponID; }

	FORCEINLINE UFUNCTION() USB_ShipOTMWidget* const GetOTMWidget() const { return OTMWidget; }
	
	// Delegates

	FSelectedWeaponUpdated SelectedWeaponUpdatedEvent;
	FDurabilityUpdatedDelegate OnDurabilityUpdated;
	FDestroyedDelegate OnDestroyed;

	// SceneComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_ThrusterModule* ThrusterModule_Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_ThrusterModule* ThrusterModule_Front;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_ThrusterModule* ThrusterModule_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_ThrusterModule* ThrusterModule_Right;

private:

	const ASB_DataManager* DataManager;

	// SceneComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_BaseModule* HullModule_01;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_BaseModule* CommandModule;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USB_ShieldModule* ShieldModule;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class USceneComponent*> AutoLockCTs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* CircleParticleCT;

	// ActorComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipCameraManager* ShipCameraManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipPowerComponent* PowerCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipMovementComponent* MovementCT;

	// Modules lists

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	TArray<class USB_ThrusterModule*> ThrusterModules;

	//UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	TInlineComponentArray<USB_WeaponModule*> WeaponModules;

	//

	UPROPERTY()
	USB_ShipOTMWidget* OTMWidget;

	UPROPERTY()
	ESB_ShipState State;

	UPROPERTY(Replicated)
	FRotator OwnerViewRotation; // ToDo : Delete, useless, did it for shield setup. // Lol

	UPROPERTY(Replicated)
	FVector OwnerViewLocation;

	UPROPERTY()
	class AActor* OwnerViewActor;

	UPROPERTY()
	uint8 SelectedWeaponID;

	UPROPERTY(ReplicatedUsing = OnRep_Durability)
	float Durability;

	//

	UFUNCTION() void OnRep_Durability();
	UFUNCTION() void Debug(float DeltaTime);

};
