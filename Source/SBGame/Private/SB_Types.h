#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SB_Types.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TSubclassOf<class ASB_Ship> ShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TSubclassOf<class AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	bool bAutoSpawnPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> BaseModuleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WeaponModuleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bIsDebugEnabled_GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bIsDebugEnabled_Ship;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bIsDebugEnabled_ShipMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bIsDebugEnabled_WeaponModule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bIsDebugEnabled_Projectile;

	FSB_GameSettings()
	{
		ShipClass = nullptr;
		AIControllerClass = nullptr;
		bAutoSpawnPlayers = false;
		BaseModuleWidgetClass = nullptr;
		WeaponModuleWidgetClass = nullptr;
		bIsDebugEnabled_GameMode = false;
		bIsDebugEnabled_Ship = false;
		bIsDebugEnabled_ShipMovement = false;
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
	float BaseMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseTurnSpeed;

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
		BaseMoveSpeed = 10000.0f;
		BaseTurnSpeed = 2.0f;
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