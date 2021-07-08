///// RZ_DamageTakenNotificationWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_DamageTakenNotificationWidget.generated.h"

UCLASS()
class RZ_UIPLUGIN_API URZ_DamageTakenNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION() 
	void Init(const class AActor* const NewTargetActor, float Damage);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	const class AActor* TargetActor;
	
};
