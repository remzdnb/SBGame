#pragma once

#include "CoreMinimal.h"
#include "SB_Types.h"
#include "GameFramework/WorldSettings.h"
#include "SB_WorldSettings.generated.h"

class ASB_PlayerStart;

UCLASS()
class ASB_WorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameMode)
	TArray<ASB_PlayerStart*> PlayerStart_0;
};
