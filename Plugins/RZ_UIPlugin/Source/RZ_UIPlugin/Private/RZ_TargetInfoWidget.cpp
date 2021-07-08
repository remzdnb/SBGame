#include "RZ_TargetInfoWidget.h"
//
#include "Components/TextBlock.h"

void URZ_TargetInfoWidget::Update_BPN_Implementation(bool bShouldOpen, const FString& NewName)
{
	NameText->SetText(FText::FromString(NewName));
}