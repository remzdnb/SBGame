///// SB_BattlePlayerController.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "SB_PlayerController.h"
#include "CoreMinimal.h"
#include "SB_BattlePlayerController.generated.h"

UCLASS()
class ASB_BattlePlayerController : public ASB_PlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;
	
	virtual void SetupInputComponent() override;

private:

	UFUNCTION()
	void UpdateViewTarget(float DeltaTime) const;

	//

	UFUNCTION() void LeftMouseButtonPressed();
	UFUNCTION() void LeftMouseButtonReleased();
	UFUNCTION() void TabKeyPressed();
};
