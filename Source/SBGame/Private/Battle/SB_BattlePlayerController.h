///// SB_BattlePlayerController.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "SB_Interfaces.h"
#include "SB_PlayerController.h"
//
#include "CoreMinimal.h"
#include "SB_BattlePlayerController.generated.h"

class ASB_BattleGameMode;
class ASB_GameState;
class ASB_PlayerState;

UCLASS()
class ASB_BattlePlayerController : public ASB_PlayerController, public ISB_ControllerInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;
	virtual void SetupInputComponent() override;

	// ControllerInterface
	
	virtual uint8 GetTeamID() override;
	virtual ASB_Ship* const SpawnAndPossessVehicle(const FTransform& SpawnTransform) override;

	//

	UFUNCTION()
	void OnDamageDealt(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType);

	UFUNCTION(Client, Reliable)
	void OnDamageDealt_Client(float PrimaryDamage, float SecondaryDamage, const FVector& HitLocation, ESB_PrimaryDamageType PrimaryDamageType);

	UFUNCTION(Server, Reliable)
	void Respawn_Server();

	//

	FORCEINLINE UFUNCTION() ASB_PlayerState* const GetPlayerState() const { return PState; }

private:

	UFUNCTION()
	void UpdateViewTarget(float DeltaTime) const;

	//

	UFUNCTION() void LeftMouseButtonPressed();
	UFUNCTION() void LeftMouseButtonReleased();
	UFUNCTION() void TabKeyPressed();

	//

	ASB_BattleGameMode* GMode;
	ASB_GameState* GState;
	ASB_PlayerState* PState;

	UPROPERTY()
	class URZ_LogWidget* LogWidget;
};
