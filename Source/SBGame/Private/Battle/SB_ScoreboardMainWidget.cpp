#include "SB_ScoreboardMainWidget.h"
#include "SB_ScoreboardTeamWidget.h"
#include "SB_GameInstance.h"
#include "SB_GameState.h"
//
#include "Components/PanelWidget.h"
#include "EngineUtils.h"

void USB_ScoreboardMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	GState = GetWorld()->GetGameState<ASB_GameState>();
	GState->OnGameStateUpdated.AddUniqueDynamic(this, &USB_ScoreboardMainWidget::Update);

	Update();
}

void USB_ScoreboardMainWidget::Update()
{
	TeamsContainerPanel->ClearChildren();
	
	for (uint8 Index = 0; Index < GState->TeamsData.Num(); Index++)
	{
		if (GState->TeamsData[Index].MaxPlayers > 0 && GState->TeamsData[Index].PlayerList.Num() != 0)
		{
			USB_ScoreboardTeamWidget* const TeamWidget = CreateWidget<USB_ScoreboardTeamWidget>(GetWorld(), GInstance->UISettings.ScoreboardTeam_WBP);
			if (TeamWidget)
			{
				TeamWidget->Init(Index);
				TeamsContainerPanel->AddChild(TeamWidget);
			}
		}
	}
}