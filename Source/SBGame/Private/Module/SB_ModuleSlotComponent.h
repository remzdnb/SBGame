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
	void SpawnModule(const FName& NewModuleDataRowName);
	
	UFUNCTION()
	void SpawnDefaultModule();

	UFUNCTION()
	void UpdateEditorMesh();

	//

	FORCEINLINE UFUNCTION() const FSB_ModuleSlotData& GetModuleSlotData() const { return ModuleSlotData; }
	FORCEINLINE UFUNCTION() class USB_BaseModule* GetSpawnedModule() const { return SpawnedModule; }

private:

	class USB_GameInstance* GInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FSB_ModuleSlotData ModuleSlotData;

	//

	//UPROPERTY()
	//class ASB_ModuleSlotActor* ModuleSlotActor;
	
	UPROPERTY()
	class USB_BaseModule* SpawnedModule;
	
	class UDataTable* BaseModuleDT;
};
