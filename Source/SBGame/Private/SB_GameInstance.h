///// SB_Ship.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SB_GameInstance.generated.h"

class ASB_DataManager;

UCLASS()
class USB_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void StartGameInstance() override;
	virtual void OnStart() override;
	virtual void Init() override;

	FORCEINLINE UFUNCTION() const class ASB_DataManager* const GetDataManager() const { return DataManager; }

private:

	const ASB_DataManager* DataManager;
};
