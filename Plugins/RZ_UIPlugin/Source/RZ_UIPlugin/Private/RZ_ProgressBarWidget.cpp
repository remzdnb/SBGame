#include "RZ_ProgressBarWidget.h"
//
#include "Components/ProgressBar.h"

void URZ_ProgressBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TargetPercent = 1.0f;
}

void URZ_ProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	LerpedProgressBar->SetPercent(FMath::Lerp(LerpedProgressBar->Percent, TargetPercent, PROGRESSBAR_LERPSPEED));
}

void URZ_ProgressBarWidget::UpdatePercent(float NewPercent)
{
	TargetPercent = NewPercent;
	BaseProgressBar->SetPercent(TargetPercent);
}