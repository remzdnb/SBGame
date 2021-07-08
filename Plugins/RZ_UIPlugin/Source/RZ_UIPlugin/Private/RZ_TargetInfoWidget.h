// URZ_TargetInfoWidget - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_TargetInfoWidget.generated.h"

UCLASS()
class RZ_UIPLUGIN_API URZ_TargetInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void Update_BPN(bool bShouldOpen, const FString& NewName);

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;

};
