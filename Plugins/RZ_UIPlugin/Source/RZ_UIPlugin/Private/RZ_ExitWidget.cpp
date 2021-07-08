#include "RZ_ExitWidget.h"
#include "Input/RZ_ButtonWidget.h"

void URZ_ExitWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	QuitToDesktopButton->OnButtonPressed.AddDynamic(this, &URZ_ExitWidget::OnQuitToDesktopButtonPressed);
	QuitToMenuButton->OnButtonPressed.AddDynamic(this, &URZ_ExitWidget::OnQuitToMenuButtonButtonPressed);
}

void URZ_ExitWidget::OnQuitToDesktopButtonPressed(uint8 ButtonID)
{
	const FString Command = "exit";
	GetOwningPlayer()->ConsoleCommand(Command, true);
}

void URZ_ExitWidget::OnQuitToMenuButtonButtonPressed(uint8 ButtonID)
{
	const FString Command = "open ASG_MenuMap";
	GetOwningPlayer()->ConsoleCommand(Command, true);
}
