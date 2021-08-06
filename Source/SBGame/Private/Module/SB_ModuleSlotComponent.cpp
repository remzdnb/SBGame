#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ModuleSlotActor.h"
#include "Module/SB_BaseModule.h"
#include "Module/Weapon/SB_BaseWeaponModule.h"
#include "Ship/SB_Ship.h"
#include "SB_GameInstance.h"
#include "SB_DataManager.h"
//
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

	DManager = GInstance->GetDataManager();
}

void USB_ModuleSlotComponent::SpawnModule()
{
	DestroyModule();

	FString ComponentName;
	USB_BaseModule* NewModule;
	
	const FSB_BaseModuleData* const BaseModuleData = GInstance->GetBaseModuleDataFromRow(ModuleSlotData.DefaultModuleRowName);
	if (BaseModuleData)
	{


		switch (BaseModuleData->ModuleType)
		{
		case ESB_ModuleType::PrimaryWeapon:
			ComponentName = "PrimaryWeapon" + FString::FromInt(ModuleSlotData.UniqueID);
			NewModule = NewObject<USB_BaseWeaponModule>(this, *ComponentName);
			break;
		default:
			ComponentName = "BaseModule" + FString::FromInt(ModuleSlotData.UniqueID);
			NewModule = NewObject<USB_BaseModule>(this, *ComponentName);
		}

		NewModule->Init(DManager, ModuleSlotData, ModuleSlotData.DefaultModuleRowName);
		NewModule->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NewModule->SetRelativeLocation(GetRelativeLocation());
		NewModule->SetRelativeRotation(GetRelativeRotation());
		NewModule->RegisterComponent();

		SpawnedModule = NewModule;
	}
	else
	{
		const FSB_BaseModuleData* const NewBaseModuleData = GInstance->GetBaseModuleDataFromRow(ModuleSlotData.DefaultModuleRowName);
		if (NewBaseModuleData)
		{
			ComponentName = "EmptyModule" + FString::FromInt(ModuleSlotData.UniqueID);
			NewModule = NewObject<USB_BaseWeaponModule>(this, *ComponentName);
		
			NewModule->Init(DManager, ModuleSlotData, "Empty");
			NewModule->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			NewModule->SetRelativeLocation(GetRelativeLocation());
			NewModule->SetRelativeRotation(GetRelativeRotation());
			NewModule->RegisterComponent();

			SpawnedModule = NewModule;
		}

	}
}

void USB_ModuleSlotComponent::DestroyModule()
{
	if (SpawnedModule)
	{
		SpawnedModule->DestroyComponent();
	}
}

void USB_ModuleSlotComponent::UpdateDemoMesh()
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
		}
	}
}
