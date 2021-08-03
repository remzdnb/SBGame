#include "SB_ScoreboardMainWidget.h"
#include "SB_ScoreboardPlayerWidget.h"
#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "SB_DataManager.h"
//
#include "Components/PanelWidget.h"
#include "EngineUtils.h"

void USB_ScoreboardMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetWorld() == nullptr)
		return;

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	GameState = GetWorld()->GetGameState<ASB_GameState>();
	if (GameState)
	{
		GameState->GameStateUpdatedEvent.AddUniqueDynamic(this, &USB_ScoreboardMainWidget::Update);
		Update(true);
	}
}

void USB_ScoreboardMainWidget::Update(bool bDummy)
{
	SpectatorTeamContainer->ClearChildren();
	ATeamContainer->ClearChildren();
	BTeamContainer->ClearChildren();

	for (auto& PlayerState : GameState->PlayerArray)
	{
		USB_ScoreboardPlayerWidget* const PlayerWidget = CreateWidget<USB_ScoreboardPlayerWidget>(GetWorld(), DataManager->UISettings.ScoreboardPlayer_WBP);
		if (PlayerWidget)
		{
			const ASB_PlayerState* const SBPlayerState = Cast<ASB_PlayerState>(PlayerState);

			PlayerWidget->Init(SBPlayerState);

			switch (SBPlayerState->GetTeam())
			{
				case 1:
					ATeamContainer->AddChild(PlayerWidget);
					break;
				case 2:
					BTeamContainer->AddChild(PlayerWidget);
					break;
				default:
					SpectatorTeamContainer->AddChild(PlayerWidget);
			}
		}
	}
}