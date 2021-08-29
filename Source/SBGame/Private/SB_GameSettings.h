#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SB_GameSettings.generated.h"

UCLASS()
class USB_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	USB_GameSettings();

	//
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	uint8 MaxTeams;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	uint8 MaxSpectators;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	TArray<FString> TeamNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	TArray<FLinearColor> TeamColors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	float FreeRespawnTimer;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSubclassOf<class AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSubclassOf<AActor> AIShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<uint8> DefaultBotCountByTeam;

	//
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TSubclassOf<AActor> CampaignPostProcess_BP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TSubclassOf<AActor> BattlePostProcess_BP;
};
