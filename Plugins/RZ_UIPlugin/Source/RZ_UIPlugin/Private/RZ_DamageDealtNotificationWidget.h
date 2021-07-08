///// RZ_DamageDealtNotificationWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_DamageDealtNotificationWidget.generated.h"

UCLASS()
class RZ_UIPLUGIN_API URZ_DamageDealtNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION() 
	void Init(const class AActor* const NewTargetActor, float Damage);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	const class AActor* TargetActor;

	//

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;
	
};
