#include "Battle/SB_ScoreboardTeamWidget.h"
#include "Battle/SB_ScoreboardPlayerWidget.h"
#include "SB_GameInstance.h"
#include "SB_GameState.h"
#include "SB_GameSettings.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void USB_ScoreboardTeamWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	GState = GetWorld()->GetGameState<ASB_GameState>();
}

void USB_ScoreboardTeamWidget::Init(uint8 NewTeamID)
{
	TeamID = NewTeamID;

	Update();
}

void USB_ScoreboardTeamWidget::Update()
{
	if (GInstance->GameSettings->TeamColors.IsValidIndex(TeamID))
		TeamBorder->SetBrushColor(GInstance->GameSettings->TeamColors[TeamID]);

	if (GInstance->GameSettings->TeamNames.IsValidIndex(TeamID))
		TeamNameText->SetText(FText::FromString(GInstance->GameSettings->TeamNames[TeamID]));
	
	PlayerCountText->SetText(FText::FromString(
		FString::FromInt(GState->TeamsData[TeamID].PlayerList.Num()) +
		" / " +
		FString::FromInt(GState->TeamsData[TeamID].MaxPlayers)
	));

	PlayerWidgetsContainerPanel->ClearChildren();
	
	for (const auto& Player : GState->TeamsData[TeamID].PlayerList)
	{
		USB_ScoreboardPlayerWidget* const PlayerWidget = CreateWidget<USB_ScoreboardPlayerWidget>(GetWorld(), GInstance->UISettings.ScoreboardPlayer_WBP);
		if (PlayerWidget)
		{
			PlayerWidget->Init(Player->PlayerState);
			PlayerWidgetsContainerPanel->AddChild(PlayerWidget);
		}
	}
}
