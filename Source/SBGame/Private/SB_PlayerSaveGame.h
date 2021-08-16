#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SB_PlayerSaveGame.generated.h"

UCLASS()
class USB_PlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	USB_PlayerSaveGame();

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString MapName;

	UPROPERTY()
	FString JoinIP;

	UPROPERTY()
	FName ShipDataRowName;
	
	UPROPERTY()
	TArray<FName> ShipConfig;
};
