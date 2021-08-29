
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SB_PlayerState.generated.h"

UCLASS()
class ASB_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ASB_PlayerState();

	virtual void PostInitializeComponents() override;

	//

	UFUNCTION()
	void SetName(bool bIsPlayer = true, const FString& NewPlayerName = FString());

	UFUNCTION()
	void SetTeamID(uint8 NewTeam);

	//

	FORCEINLINE UFUNCTION() class USB_PlayerSaveGame* GetPlayerSaveGame() const {return PSaveGame;}
	FORCEINLINE UFUNCTION() const FString& GetDisplayName() const { return DisplayName; }
	FORCEINLINE UFUNCTION() uint8 GetTeamID() const { return TeamID; }

private:

	class ASB_GameState* GState;

	//
	
	UPROPERTY()
	class USB_PlayerSaveGame* PSaveGame;

	//

	UPROPERTY()
	FString DisplayName;

	UPROPERTY(Replicated)
	uint8 TeamID;

	UPROPERTY(Replicated)
	uint32 KillCount;

	UPROPERTY(Replicated)
	uint32 DeathCount;
};
