#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SB_Types.generated.h"

#define TRACE_OVERLAP ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class ESB_ShipState : uint8
{
	Ready,
	Destroyed
};

UENUM(BlueprintType)
enum class ESB_ModuleType : uint8
{
	Power,
	Thruster,
	Weapon,
	Shield
};

#pragma region +++++ Game Data ...

USTRUCT(BlueprintType)
struct FSB_GameSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ASB_Ship> ShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoSpawnPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ModuleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled_Ship;

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
		ShipClass = nullptr;
		AIControllerClass = nullptr;
		bAutoSpawnPlayers = false;
		ModuleWidgetClass = nullptr;
		bIsDebugEnabled_GameMode = false;
		bIsDebugEnabled_Ship = false;
		bIsDebugEnabled_ShipMovement = false;
		bIsDebugEnabled_ThrusterModule = false;
		bIsDebugEnabled_WeaponModule = false;
		bIsDebugEnabled_Projectile = false;
	}
};

USTRUCT(BlueprintType)
struct FSB_CameraSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DefaultOffset;

	FSB_CameraSettings()
	{
	}
};

USTRUCT(BlueprintType)
struct FSB_ShipSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnSpeed;

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
		TurnSpeed = 2.0f;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TSubclassOf<class ASB_Shield> ShieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* SetupMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInterface* DeployedMaterial;

	FSB_ShieldSettings()
	{
		ShieldClass = nullptr;
		SetupMaterial = nullptr;
		DeployedMaterial = nullptr;
	}
};

#pragma endregion

#pragma region +++++ Module Data ...

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
	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RepairAmount;

	FSB_BaseModuleData()
	{
		DisplayName = "Default";
		DisplayTexture = nullptr;
		ModuleType = ESB_ModuleType::Weapon;
		SkeletalMesh = nullptr;
		AnimInstance = nullptr;
		MaxPower = 0;
		MaxDurability = 1000;
		RepairAmount = 10;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* MuzzleParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MuzzleParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundCue* FireSound;

	FSB_WeaponModuleData()
	{
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