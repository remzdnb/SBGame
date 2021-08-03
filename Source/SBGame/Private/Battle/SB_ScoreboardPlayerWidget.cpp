#include "SB_ScoreboardPlayerWidget.h"
#include "SB_PlayerState.h"
//
#include "Components/TextBlock.h"

void USB_ScoreboardPlayerWidget::Init(const ASB_PlayerState* const PlayerState)
{
	if (PlayerState)
	{
		PlayerNameText->SetText(FText::FromString(PlayerState->GetDisplayName()));
		TeamText->SetText(FText::FromString(FString::FromInt(PlayerState->GetTeam())));
	}
}