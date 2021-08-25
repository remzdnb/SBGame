#include "SB_ScoreboardPlayerWidget.h"
#include "SB_PlayerState.h"
//
#include "Components/TextBlock.h"

void USB_ScoreboardPlayerWidget::Init(const APlayerState* const NewPlayerState)
{
	const ASB_PlayerState* const PlayerState = Cast<ASB_PlayerState>(NewPlayerState);
	if (PlayerState)
	{
		PlayerNameText->SetText(FText::FromString(PlayerState->GetDisplayName()));
	}
}