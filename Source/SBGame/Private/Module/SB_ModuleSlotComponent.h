///// SB_ModuleSlotComponent.h - RemzDNB
///// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "SB_ModuleSlotComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USB_ModuleSlotComponent : public UChildActorComponent
{
	GENERATED_BODY()

public:

	USB_ModuleSlotComponent();

	virtual void InitializeComponent() override;

	//

	UFUNCTION()
	void SpawnModule();

	UFUNCTION()
	void DestroyModule();

	UFUNCTION()
	void UpdateDemoMesh();

	//

	FORCEINLINE UFUNCTION() const FSB_ModuleSlotData& GetModuleSlotData() const { return ModuleSlotData; }
	FORCEINLINE UFUNCTION() const class USB_BaseModule* GetSpawnedModule() const { return SpawnedModule; }

private:

	class USB_GameInstance* GInstance;
	class ASB_DataManager* DManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FSB_ModuleSlotData ModuleSlotData;

	//

	//UPROPERTY()
	//class ASB_ModuleSlotActor* ModuleSlotActor;
	
	UPROPERTY()
	class USB_BaseModule* SpawnedModule;

	
	class UDataTable* BaseModuleDT;
};
