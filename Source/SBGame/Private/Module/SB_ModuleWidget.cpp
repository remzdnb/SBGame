#include "Module/SB_ModuleWidget.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameInstance.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

#include "EngineUtils.h"

USB_ModuleWidget::USB_ModuleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(UUserWidget::GetWorld()));

	/*static ConstructorHelpers::FObjectFinder<UDataTable> FoundDT(TEXT("DataTable'/Game/Data/SB_BaseModuleData_DT.SB_BaseModuleData_DT'"));
	if (FoundDT.Succeeded())
	{
		BaseModuleDT = FoundDT.Object;
	}*/
}

void USB_ModuleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply USB_ModuleWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}

	//
	
	OnPressed.Broadcast(SlotData.UniqueID);

	return FReply::Handled();
}

FReply USB_ModuleWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void USB_ModuleWidget::Update(const FSB_ModuleSlotData& NewSlotData, const USB_BaseModule* const NewModuleRef)
{
	SlotData = NewSlotData;
	ModuleRef = NewModuleRef;
	
	SlotID->SetText(FText::FromString(FString::FromInt(NewSlotData.UniqueID)));
	SlotName->SetText(FText::FromString(NewSlotData.DisplayName));

	if (NewModuleRef)
	{
		ModuleName->SetText(FText::FromString(NewModuleRef->GetBaseModuleData()->DisplayName.ToString()));
	}
	
	//const FSB_BaseModuleData* const BaseModuleData = DataManager->GetBaseModuleDataFromRow(NewModuleDataRowName);
	/*if (BaseModuleDT)
	{
		UE_LOG(LogTemp, Warning, TEXT("USB_ModuleWidget::Update : 2"));
		
		const FString ContextString;
		const FSB_BaseModuleData* const BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(NewModuleDataRowName, ContextString);
		if (BaseModuleData)
		{
			UE_LOG(LogTemp, Warning, TEXT("USB_ModuleWidget::Update : 3"));
			
			ModuleName->SetText(FText::FromString(BaseModuleData->DisplayName.ToString()));
		}
	}*/

	
	/*BaseModuleRef = ModuleRef;
	ModuleID = NewModuleID;
	DataRowName = NewDataRowName;

	if (BaseModuleRef.IsValid())
	{
	BaseModuleRef->OnSelectionUpdated.AddUniqueDynamic(this, &USB_ModuleWidget::OnSelectionUpdatedBPI);
	BaseModuleRef->OnStateUpdated.AddUniqueDynamic(this, &USB_ModuleWidget::OnStateUpdatedBPI);
	BaseModuleRef->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_ModuleWidget::OnDurabilityUpdated);
		
	const FSB_BaseModuleData* const ModuleRefData = BaseModuleRef->GetBaseModuleData();
	if (ModuleRefData)
	{
	ModuleSlotIDText->SetText(FText::FromString(FString::FromInt(BaseModuleRef->GetModuleSlotData().UniqueID)));
	ModuleSlotTypeText->SetText(FText::FromString(SB_UtilityLibrary::GetEnumAsString("ESB_ModuleType", ModuleRefData->ModuleType)));
	ModuleRowNameText->SetText(FText::FromString(ModuleRefData->DisplayName.ToString()));
	ModuleImage->SetBrushFromTexture(ModuleRefData->DisplayTexture);
	}
	}
	else
	{
	if (ModuleData)
	{
	ModuleSlotIDText->SetText(FText::FromString(FString::FromInt(ModuleID)));
	ModuleSlotTypeText->SetText(FText::FromString(SB_UtilityLibrary::GetEnumAsString("ESB_ModuleType", ModuleData->ModuleType)));
	ModuleRowNameText->SetText(FText::FromString(ModuleData->DisplayName.ToString()));
	ModuleImage->SetBrushFromTexture(ModuleData->DisplayTexture);
	}
	}*/
}

void USB_ModuleWidget::OnDurabilityUpdated(float NewDurability)
{
	ModuleDurabilityProgressBar->SetPercent(NewDurability / ModuleRef->GetBaseModuleData()->MaxDurability);
}