#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SB_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateUpdated, bool, bDummy);

UCLASS()
class ASB_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	FGameStateUpdated GameStateUpdatedEvent;

	//

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	//

	void BroadcastGameStateUpdated();
};
