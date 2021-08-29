#pragma once

#include "CoreMinimal.h"
#include "SB_PlayerStart.h"
#include "Engine/DataTable.h"
#include "SB_Types.generated.h"

UENUM(BlueprintType)
enum class ESB_GameType : uint8
{
	Campaign,
	Battle
};

UENUM(BlueprintType)
enum class ESB_GamePhase : uint8
{
	WaitingForPlayers,
	Ready,
	Starting,
	Playing,
	Stopping
};

//

UENUM(BlueprintType)
enum class ESB_ModuleType : uint8
{
	Empty,
	Hull,
	Command,
	Thruster,
	Weapon,
	Shield
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
	Destroyed,
	Demo // Disable turret rotation
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
struct FSB_ShieldSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ShieldBP;

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
	TSubclassOf<class UUserWidget> HUDMain_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> HUDWeapon_WBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> HUDVehicleOTM_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> HUDDamageMarker_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ScoreboardMain_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ScoreboardTeam_WBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ScoreboardPlayer_WBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> Cursor_WBP;
};

USTRUCT(BlueprintType)
struct FSB_AISettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> AIShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectionUpdateRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionDetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionDetectionSphereRadius;

	FSB_AISettings()
	{
		AIControllerClass = nullptr;
		AIShipClass = nullptr;
		DetectionUpdateRate = 0.5f;
		CollisionDetectionRange = 35000.0f;
		CollisionDetectionSphereRadius = 1000.0f;
	}
};

USTRUCT(BlueprintType)
struct FSB_DebugSettings
{
	GENERATED_USTRUCT_BODY()

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

	FSB_DebugSettings()
	{
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

#pragma endregion

#pragma region +++++ Data ...

USTRUCT()
struct FSB_TeamData
{
	GENERATED_USTRUCT_BODY()
	
	uint8 TeamID;
	uint8 MaxPlayers;
	TArray<TWeakObjectPtr<ASB_PlayerStart>> PlayerStartList;
	TArray<TWeakObjectPtr<AController>> PlayerList;
	
	FSB_TeamData()
	{
		TeamID = 0;
		MaxPlayers = 0;
	}
};

USTRUCT(BlueprintType)
struct FSB_VehicleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* DisplayTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> Vehicle_BP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> DestructibleVehicle_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* OnDestroyedParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OnDestroyedParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxDurability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveInertia;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnInertia;
 
	FSB_VehicleData()
	{
		DisplayName = "ShipName";
		DisplayTexture = nullptr;
		Vehicle_BP = nullptr;
		DestructibleVehicle_BP = nullptr;
		OnDestroyedParticle = nullptr;
		OnDestroyedParticleScale = 10.0f;
		MaxDurability = 10000.0f;
		MaxMoveSpeed = 5000.0f;
		MaxAcceleration = 1024.0f;
		MoveInertia = 1.0;
		TurnRate = 2.0f;
		TurnInertia = 0.1f;
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
	TArray<FName> Tags;
 
	FSB_ModuleSlotData()
	{
		UniqueID = 0;
		Type = ESB_ModuleType::Hull;
		DisplayName = "SlotName";
		DefaultModuleRowName = "DataRowName";
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
		ModuleType = ESB_ModuleType::Empty;
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
struct FSB_WeaponModuleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	ESB_WeaponType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float FireRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float RotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	uint8 MuzzleCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	class UParticleSystem* MuzzleParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float MuzzleParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	class USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float ImpactParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	class USoundCue* ImpactSound;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ProjectileLifeSpan;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	TSubclassOf<AActor> BeamBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	UParticleSystem* BeamParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	FVector BeamScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float BeamSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float BeamTickRate;

	FSB_WeaponModuleData()
	{
		Type = ESB_WeaponType::Projectile;
		Damage = 10.0f;
		Range = 10000.0f;
		FireRate = 0.5f;
		RotationRate = 0.1f;
		MuzzleCount = 1;
		MuzzleParticle = nullptr;
		MuzzleParticleScale = 10.0f;
		FireSound = nullptr;
		ImpactParticle = nullptr;
		ImpactParticleScale = 10.0f;
		ImpactSound = nullptr;
		ProjectileBP = nullptr;
		ProjectileLifeSpan = 5.0f;
		BeamBP = nullptr;
		BeamParticle = nullptr;
		BeamScale = FVector(5.0f);
		BeamSpeed = 2.0;
		BeamTickRate = 0.2f;
	}
};

#pragma endregion