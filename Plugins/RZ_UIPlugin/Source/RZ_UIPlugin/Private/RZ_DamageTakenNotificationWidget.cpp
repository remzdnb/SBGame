#include "RZ_DamageTakenNotificationWidget.h"
//
#include "Components/TextBlock.h"

void URZ_DamageTakenNotificationWidget::Init(const class AActor* const NewTargetActor, float Damage)
{
	TargetActor = NewTargetActor;
}