#include "RZ_HomeWidget.h"
//
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URZ_HomeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*if (PController->GetRZESaveGame())
	{
		PlayerNameEText->SetText(FText::FromString(PController->GetRZESaveGame()->PlayerData.PlayerName));
	}

	PlayerNameEText->OnTextValidated.AddDynamic(this, &URZ_HomeWidget::OnPlayerNameChanged);*/
	/*LoadMapButton->OnButtonPressed.AddDynamic(this, &URZ_HomeWidget::OnLoadMapButtonPressed);
	JoinServerButton->OnButtonPressed.AddDynamic(this, &URZ_HomeWidget::OnJoinServerButtonPressed);
	CreateServerButton->OnButtonPressed.AddDynamic(this, &URZ_HomeWidget::OnCreateServerButtonPressed);*/
}

void URZ_HomeWidget::OnPlayerNameChanged(const FText& Text)
{
	/*if (PController.IsValid())
	{
		PController->GetRZESaveGame()->PlayerData.PlayerName = *Text.ToString();

		UGameplayStatics::SaveGameToSlot(PController->GetRZESaveGame(), "RZESaveGame", 0);

		UE_LOG(LogTemp, Warning, TEXT("PlayerName Saved"));
	}*/
}

void URZ_HomeWidget::OnLoadMapButtonPressed(uint8 ButtonID)
{
	/*const FString Command = "open " + MapNameEText->GetText().ToString();
	GetOwningPlayer()->ConsoleCommand(Command, true);*/
}

void URZ_HomeWidget::OnJoinServerButtonPressed(uint8 ButtonID)
{
}

void URZ_HomeWidget::OnCreateServerButtonPressed(uint8 ButtonID)
{
}
