#include "SB_HomeWidget.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
#include "RZ_EditableTextWidget.h"
#include "RZ_ButtonWidget.h"
//
#include "Kismet/GameplayStatics.h"

void USB_HomeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	if (GInstance->GetSaveGame())
	{
		PlayerNameEText->SetText(FText::FromString(GInstance->GetSaveGame()->PlayerName));
		MapNameEText->SetText(FText::FromString(GInstance->GetSaveGame()->MapName));
		JoinIPEText->SetText(FText::FromString(GInstance->GetSaveGame()->JoinIP));
	}
	
	PlayerNameEText->OnTextValidated.AddDynamic(this, &USB_HomeWidget::OnPlayerNameChanged);
	MapNameEText->OnTextValidated.AddUniqueDynamic(this, &USB_HomeWidget::OnMapNameChanged);
	JoinIPEText->OnTextValidated.AddUniqueDynamic(this, &USB_HomeWidget::OnMapNameChanged);
	LoadMapButton->OnButtonPressed.AddDynamic(this, &USB_HomeWidget::OnLoadMapButtonPressed);
	JoinServerButton->OnButtonPressed.AddDynamic(this, &USB_HomeWidget::OnJoinServerButtonPressed);
	CreateServerButton->OnButtonPressed.AddDynamic(this, &USB_HomeWidget::OnCreateServerButtonPressed);
}

void USB_HomeWidget::OnPlayerNameChanged(const FText& PlayerNameText)
{
	GInstance->GetSaveGame()->PlayerName = PlayerNameText.ToString();
	GInstance->ApplySaveGame();
}

void USB_HomeWidget::OnMapNameChanged(const FText& MapNameText)
{
	GInstance->GetSaveGame()->MapName = MapNameText.ToString();
	GInstance->ApplySaveGame();
}

void USB_HomeWidget::OnJoinIPChanged(const FText& JoinIPText)
{
	GInstance->GetSaveGame()->JoinIP = JoinIPText.ToString();
	GInstance->ApplySaveGame();
}

void USB_HomeWidget::OnLoadMapButtonPressed(uint8 ButtonID)
{
	const FString Command = "open " + MapNameEText->GetText().ToString();
	
	GetOwningPlayer()->ConsoleCommand(Command, true);
}

void USB_HomeWidget::OnJoinServerButtonPressed(uint8 ButtonID)
{
}

void USB_HomeWidget::OnCreateServerButtonPressed(uint8 ButtonID)
{
}
