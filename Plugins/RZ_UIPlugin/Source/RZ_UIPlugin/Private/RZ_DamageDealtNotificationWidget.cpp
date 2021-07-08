#include "RZ_DamageDealtNotificationWidget.h"
//
#include "Components/TextBlock.h"

void URZ_DamageDealtNotificationWidget::Init(const class AActor* const NewTargetActor, float Damage)
{
	TargetActor = NewTargetActor;
	DamageText->SetText(FText::FromString(FString::SanitizeFloat(Damage)));
}