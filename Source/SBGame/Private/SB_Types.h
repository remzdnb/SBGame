#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SB_Types.generated.h"

#define TRACE_OVERLAP ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class ESB_GameType : uint8
{
	Campaign,
	Battle
};

/*UENUM(BlueprintType)
enum class ESB_ModuleSlotType : uint8
{
	Hull,
	Command,
	Thruster_Back,
	Thruster_Front,
	Thruster_Left,
	Thruster_Right,
	Weapon_Primary,
	Weapon_Auxiliary,
	Shield
};*/

UENUM(BlueprintType)
enum class ESB_ModuleType : uint8
{
	Hull,
	Command,
	Thruster,
	PrimaryWeapon,
	AuxiliaryWeapon,
	Shield
};

UENUM(BlueprintType)
enum class ESB_ThrusterType : uint8
{
	Back,
	Front,
	Left,
	Right
};

UENUM(BlueprintType)
enum class ESB_WeaponType : uint8
{
	Trace,
	Projectile,
	Beam
};

UENUM(BlueprintType)
enum class ESB_PrimaryDamageType : uint8
{
	Ship,
	Shield,
	Drone
};

//

UENUM(BlueprintType)
enum class ESB_ShipState : uint8
{
	Ready,
	Destroyed
};

UENUM(BlueprintType)
enum class ESB_ModuleState : uint8
{
	Ready,
	Repairing
};

UENUM(BlueprintType)
enum class ESB_ShieldState : uint8
{
	Ready,
	Deployed,
	Cooldown
};

#pragma region +++++ Settings ...

USTRUCT(BlueprintType)
struct FSB_GameSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESB_GameType GameType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ASB_Ship> ShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> DestructibleShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoSpawnPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_AI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_Ship;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_ShipCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_ShipMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_ThrusterModule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_WeaponModule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_Projectile;

	FSB_GameSettings()
	{
		GameType = ESB_GameType::Battle;
		ShipClass = nullptr;
		AIControllerClass = nullptr;
		bAutoSpawnPlayers = false;
		bIsDebugEnabled_GameMode = false;
		bIsDebugEnabled_PlayerController = false;
		bIsDebugEnabled_AI = false;
		bIsDebugEnabled_Ship = false;
		bIsDebugEnabled_ShipCamera = false;
		bIsDebugEnabled_ShipMovement = false;
		bIsDebugEnabled_ThrusterModule = false;
		bIsDebugEnabled_WeaponModule = false;
		bIsDebugEnabled_Projectile = false;
	}
};

USTRUCT(BlueprintType)
struct FSB_ShipSettings
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TArray<FSB_ModuleSlotData> CarrierShipConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnInertia;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* DestroyedMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* DestroyedParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DestroyedParticleScale;

	FSB_ShipSettings()
	{
		MoveSpeed = 5000.0f;
		TurnRate = 2.0f;
		TurnInertia = 0.1f;
		MaxDurability = 10000.0f;
		DestroyedMaterial = nullptr;
		DestroyedParticle = nullptr;
		DestroyedParticleScale = 10.0f;
	}
};

USTRUCT(BlueprintType)
struct FSB_ShieldSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ASB_Shield> ShieldBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* ShieldMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector ShieldMeshDefaultSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector ShieldMeshScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RegenRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RegenAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeployCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* SetupMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* DeployedMaterial;

	FSB_ShieldSettings()
	{
		ShieldBP = nullptr;
		ShieldMesh = nullptr;
		ShieldMeshDefaultSize = FVector(200.0f, 200.0f, 100.0f);
		ShieldMeshScale = FVector(90.0f, 90.0f, 50.0f);
		MaxDurability = 2000.0f;
		RegenRate = 2.0f;
		RegenAmount = 100.0f;
		DeployCooldown = 5.0f;
		SetupMaterial = nullptr;
		DeployedMaterial = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FSB_UISettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<class UUserWidget>> BattleMenuWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<class UUserWidget>> CampaignMenuWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> Cursor_WBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> BattleMenu_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ScoreboardMain_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ScoreboardPlayer_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> BattleHUD_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> Module_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ShipOTM_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> DamageMarker_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> Button_Small_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> Button_Large_WBP;
};

USTRUCT(BlueprintType)
struct FSB_AISettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectionUpdateRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionDetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionDetectionSphereRadius;

	FSB_AISettings()
	{
		DetectionUpdateRate = 0.5f;
		CollisionDetectionRange = 35000.0f;
		CollisionDetectionSphereRadius = 1000.0f;
	}
};

#pragma region +++++ Data ...

USTRUCT(BlueprintType)
struct FSB_ShipData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ASB_Ship> ShipBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> DestructibleShipBP;
 
	FSB_ShipData()
	{
		DisplayName = "ShipName";
		ShipBP = nullptr;
		DestructibleShipBP = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FSB_ModuleSlotData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 UniqueID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESB_ModuleType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DefaultModuleRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldUpdateModule; // is modifiable module
 
	FSB_ModuleSlotData()
	{
		UniqueID = 0;
		Type = ESB_ModuleType::Hull;
		DisplayName = "SlotName";
		DefaultModuleRowName = "DataRowName";
		bShouldUpdateModule = true;
	}
};

USTRUCT(BlueprintType)
struct FSB_BaseModuleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* DisplayTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESB_ModuleType ModuleType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSelectable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector WorldScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UAnimInstance> AnimInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RepairAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShipDamageModifier;

	FSB_BaseModuleData()
	{
		DisplayName = "Default";
		DisplayTexture = nullptr;
		ModuleType = ESB_ModuleType::PrimaryWeapon;
		bIsSelectable = false;
		SkeletalMesh = nullptr;
		WorldScale = FVector(1.0f);
		AnimInstance = nullptr;
		MaxDurability = 1000;
		RepairAmount = 10;
		ShipDamageModifier = 0.0f;
	}
};

USTRUCT(BlueprintType)
struct FSB_ThrusterModuleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> ExhaustSocketNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* ExhaustParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExhaustParticleScale;

	FSB_ThrusterModuleData()
	{
		ExhaustParticle = nullptr;
		ExhaustParticleScale = 1.0f;
	}
};

USTRUCT(BlueprintType)
struct FSB_BaseWeaponModuleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsPrimary;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESB_WeaponType Type;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MuzzleCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* MuzzleParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MuzzleParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundCue* FireSound;

	FSB_BaseWeaponModuleData()
	{
		bIsPrimary = true;
		Type = ESB_WeaponType::Projectile;
		RotationRate = 1.0f;
		FireRate = 0.5f;
		ProjectileBP = nullptr;
		MuzzleCount = 1;
		MuzzleParticle = nullptr;
		MuzzleParticleScale = 10.0f;
		FireSound = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FSB_ProjectileData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LifeSpan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ImpactParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundCue* ImpactSound;

	FSB_ProjectileData()
	{
		Damage = 10.0f;
		LifeSpan = 5.0f;
		ImpactParticle = nullptr;
		ImpactParticleScale = 10.0f;
		ImpactSound = nullptr;
	}
};

#pragma endregion