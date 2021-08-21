#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_ModuleSlotActor.h"
#include "Module/SB_BaseModule.h"
#include "Module/Weapon/SB_TraceWeaponModule.h"
#include "Module/Weapon/SB_ProjectileWeaponModule.h"
#include "Module/Weapon/SB_BeamWeaponModule.h"
#include "Vehicle/SB_Vehicle.h"
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

USB_BaseModule* const USB_ModuleSlotComponent::SpawnModule(const FName& NewModuleDataRowName, bool bSpawnEmptyModule)
{
	if (SpawnedModule)
		SpawnedModule->DestroyComponent();
	
	const FSB_BaseModuleData* const BaseModuleData = GInstance->GetBaseModuleDataFromRow(NewModuleDataRowName);
	if (BaseModuleData)
	{
		const FString ComponentName = NewModuleDataRowName.ToString() + FString::FromInt(ModuleSlotData.UniqueID);
		USB_BaseModule* NewModule = nullptr;
		
		if (BaseModuleData->ModuleType == ESB_ModuleType::Weapon)
		{
			const FSB_WeaponModuleData* const WeaponData = GInstance->GetWeaponModuleDataFromRow(NewModuleDataRowName);
			if (WeaponData->Type == ESB_WeaponType::Trace)
			{
				NewModule = NewObject<USB_TraceWeaponModule>(this, *ComponentName);
			}
			else if (WeaponData->Type == ESB_WeaponType::Projectile)
			{
				NewModule = NewObject<USB_ProjectileWeaponModule>(this, *ComponentName);
			}
			else if (WeaponData->Type == ESB_WeaponType::Beam)
			{
				NewModule = NewObject<USB_BeamWeaponModule>(this, *ComponentName);
			}
		}
		else if (BaseModuleData->ModuleType == ESB_ModuleType::Thruster)
		{
			NewModule = NewObject<USB_ThrusterModule>(this, *ComponentName);
		}
		else
		{
			if (BaseModuleData->ModuleType == ESB_ModuleType::Empty && bSpawnEmptyModule)
			{
				if (bSpawnEmptyModule)
					NewModule = NewObject<USB_BaseModule>(this, *ComponentName);
			}
			else
			{
				NewModule = NewObject<USB_BaseModule>(this, *ComponentName);
			}
		}
		
		NewModule->Init(ModuleSlotData, NewModuleDataRowName);
		NewModule->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NewModule->SetRelativeLocation(GetRelativeLocation());
		NewModule->SetRelativeRotation(GetRelativeRotation());
		NewModule->RegisterComponent();

		SpawnedModule = NewModule;
		return SpawnedModule;
	}

	return nullptr;
}

USB_BaseModule* const USB_ModuleSlotComponent::SpawnDefaultModule(bool bSpawnEmptyModule)
{
	return SpawnModule(ModuleSlotData.DefaultModuleRowName, bSpawnEmptyModule);
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
