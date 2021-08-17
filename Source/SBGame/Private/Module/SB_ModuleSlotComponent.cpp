#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ModuleSlotActor.h"
#include "Module/SB_BaseModule.h"
#include "Module/Weapon/SB_BaseWeaponModule.h"
#include "Ship/SB_Ship.h"
#include "SB_GameInstance.h"
//
#include "SB_ThrusterModule.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

USB_ModuleSlotComponent::USB_ModuleSlotComponent()
{
	SetChildActorClass(ASB_ModuleSlotActor::StaticClass());

	GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDT(TEXT("DataTable'/Game/Data/SB_BaseModuleData_DT.SB_BaseModuleData_DT'"));
	if (FoundDT.Succeeded())
	{
		BaseModuleDT = FoundDT.Object;
	}
}

void USB_ModuleSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USB_ModuleSlotComponent::SpawnModule(const FName& NewModuleDataRowName)
{
	if (SpawnedModule)
		SpawnedModule->DestroyComponent();
	
	const FSB_BaseModuleData* const BaseModuleData = GInstance->GetBaseModuleDataFromRow(NewModuleDataRowName);
	if (BaseModuleData)
	{
		const FString ComponentName = NewModuleDataRowName.ToString() + FString::FromInt(ModuleSlotData.UniqueID);
		USB_BaseModule* NewModule;
		
		if (BaseModuleData->ModuleType == ESB_ModuleType::PrimaryWeapon ||
			BaseModuleData->ModuleType == ESB_ModuleType::AuxiliaryWeapon)
		{
			NewModule = NewObject<USB_BaseWeaponModule>(this, *ComponentName);
		}
		else if (BaseModuleData->ModuleType == ESB_ModuleType::Thruster)
		{
			NewModule = NewObject<USB_ThrusterModule>(this, *ComponentName);
		}
		else
		{
			NewModule = NewObject<USB_BaseModule>(this, *ComponentName);
		}
		
		NewModule->Init(ModuleSlotData, NewModuleDataRowName);
		NewModule->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NewModule->SetRelativeLocation(GetRelativeLocation());
		NewModule->SetRelativeRotation(GetRelativeRotation());
		NewModule->RegisterComponent();

		SpawnedModule = NewModule;
	}
}

void USB_ModuleSlotComponent::SpawnDefaultModule()
{
	SpawnModule(ModuleSlotData.DefaultModuleRowName);
}

void USB_ModuleSlotComponent::UpdateEditorMesh()
{
	if (IsValid(BaseModuleDT) == false)
		return;

	const FString ContextString;
	const FSB_BaseModuleData* const BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(ModuleSlotData.DefaultModuleRowName, ContextString);//GInstance->GetBaseModuleDataFromRow(ModuleSlotData.DefaultModuleRowName);
	if (BaseModuleData)
	{
		ASB_ModuleSlotActor* const ModuleSlotActor = Cast<ASB_ModuleSlotActor>(GetChildActor());
		if (ModuleSlotActor)
		{
			ModuleSlotActor->DemoMesh->SetSkeletalMesh(BaseModuleData->SkeletalMesh);
			ModuleSlotActor->DemoMesh->SetRelativeScale3D(BaseModuleData->WorldScale);
		}
	}
}
