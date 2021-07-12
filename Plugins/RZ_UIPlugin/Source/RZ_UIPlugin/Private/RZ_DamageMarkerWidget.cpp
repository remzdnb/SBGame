#include "RZ_DamageMarkerWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "TimerManager.h"

void URZ_DamageMarkerWidget::Init(const FVector& NewHitLocation, float Damage)
{
	HitLocation = NewHitLocation;
	DamageText->SetText(FText::FromString(FString::FromInt(Damage)));
	RandXSeed = FMath::RandRange(DAMAGEMARKER_MAXRANDSEED * -1, DAMAGEMARKER_MAXRANDSEED);

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &URZ_DamageMarkerWidget::Destruct, DAMAGEMARKER_DELAY, false, 0.0f);

	PlayAnimationBPI(DAMAGEMARKER_DELAY);
}

void URZ_DamageMarkerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D TargetScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), HitLocation + FVector(0.0f, 0.0f, DAMAGEMARKER_ZOFFSET), TargetScreenPosition, false);
	TargetScreenPosition.X = TargetScreenPosition.X - MainPanel->GetDesiredSize().X / 2 + RandXSeed;
	TargetScreenPosition.Y = TargetScreenPosition.Y - MainPanel->GetDesiredSize().Y / 2;

	MainPanel->SetRenderTranslation(TargetScreenPosition);
}
