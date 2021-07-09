///// SB_Ship.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SB_Ship.generated.h"

#define DEFAULTCAPSULESIZE 7000.0f
#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, 0.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)
#define MAXAUTOLOCKCOMPONENTS 4

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyed, const APlayerState* const, Instigator);

UCLASS()
class ASB_Ship : public ACharacter
{
	GENERATED_BODY()

public:

	ASB_Ship(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FDestroyed DestroyedEvent;

	//

	UFUNCTION()
	void UpdateOwnerViewData(const FRotator& NewOwnerViewRotation, const FVector& NewOwnerViewLocation, class AActor* const NewOwnerViewActor);

	/*UFUNCTION(Server, Reliable)
	void UpdateOwnerViewData_Server(const FRotator& NewOwnerViewRotation, const FVector& NewOwnerViewLocation, class AActor* const NewOwnerViewActor);*/

	UFUNCTION()
	void SelectWeapon(uint8 WeaponID);

	UFUNCTION(Server, Reliable)
	void SelectWeapon_Server(uint8 WeaponID);

	UFUNCTION()
	void StartFireSelectedWeapon();

	UFUNCTION(Server, Reliable)
	void StartFireSelectedWeapon_Server();

	UFUNCTION()
	void StopFireSelectedWeapon();

	UFUNCTION(Server, Reliable)
	void StopFireSelectedWeapon_Server();

	UFUNCTION()
	void StartAutoLockSelectedWeapon();

	UFUNCTION()
	void StopAutoLockSelectedWeapon();

	UFUNCTION()
	void UpdateState(ESB_ShipState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateState_Multicast(ESB_ShipState NewState);

	UFUNCTION()
	void ToggleOutline(bool bNewIsVisible);

	// Setters / Getters

	FORCEINLINE UFUNCTION() const class ASB_DataManager* const GetDataManager() const { return DataManager; }
	//
	FORCEINLINE UFUNCTION() class USB_ShipMovementComponent* const GetShipMovementCT() const { return MovementCT; }
	FORCEINLINE UFUNCTION() class USB_ShipCombatComponent* const GetShipCombatCT() const { return CombatCT; }
	FORCEINLINE UFUNCTION() class USB_ShipCameraComponent* const GetShipCameraCT() const { return ShipCameraCT; }
	//
	FORCEINLINE UFUNCTION() TArray<class USB_BaseModule*> GetModules() const { return BaseModules; }
	FORCEINLINE UFUNCTION() TArray<class USB_ThrusterModule*> GetThrusterModules() const { return ThrusterModules; }
	FORCEINLINE UFUNCTION() TArray<class USB_WeaponModule*> GetWeaponModules() const { return WeaponModules; }
	FORCEINLINE UFUNCTION() TArray<class USB_PowerModule*> GetPowerModules() const { return PowerModules; }
	FORCEINLINE UFUNCTION() class USB_ShieldModule* const GetShieldModule() const { return ShieldModule; }
	FORCEINLINE UFUNCTION() TArray<class USceneComponent*> GetAutoLockCTs() const { return AutoLockCTs; }
	//
	FORCEINLINE UFUNCTION() ESB_ShipState GetState() const { return State; }
	FORCEINLINE UFUNCTION() const FRotator& GetOwnerViewRotation() const { return OwnerViewRotation; }
	FORCEINLINE UFUNCTION() const FVector& GetOwnerViewLocation() const { return OwnerViewLocation; }

private:

	class ASB_DataManager* DataManager;

	// Main SceneComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBoxCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArmCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* CircleParticleCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class USceneComponent*> AutoLockCTs;

	// ModuleSlot SceneComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ModuleSlot* LeftThrusterSlotCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ModuleSlot* BackThrusterSlotCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ModuleSlot* RightThrusterSlotCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ModuleSlot* ShieldSlot_CT;

	// ActorComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipCameraComponent* ShipCameraCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipPowerComponent* PowerCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipMovementComponent* MovementCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USB_ShipCombatComponent* CombatCT;

	// Modules lists

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	TArray<class USB_BaseModule*> BaseModules;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	TArray<class USB_PowerModule*> PowerModules;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	TArray<class USB_ThrusterModule*> ThrusterModules;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	TArray<class USB_WeaponModule*> WeaponModules;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USB_ShieldModule* ShieldModule;

	// 

	UPROPERTY()
	ESB_ShipState State;

	UPROPERTY(Replicated)
	FRotator OwnerViewRotation; // ToDo : Delete, useless, did it for shield setup.

	UPROPERTY(Replicated)
	FVector OwnerViewLocation;

	UPROPERTY(Replicated)
	class AActor* OwnerViewActor;

	UPROPERTY()
	uint8 SelectedWeaponID;

	//

	UFUNCTION()
	void Debug(float DeltaTime);

};
