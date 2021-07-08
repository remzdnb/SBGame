// ARZ_UIPluginDataManager - Written by RemzDNB
// Acts as an "interface" between code and project data/references.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RZ_UIPluginDataManager.generated.h"

UCLASS(Blueprintable)
class RZ_UIPLUGIN_API ARZ_UIPluginDataManager : public AInfo
{
	GENERATED_BODY()

public:

	ARZ_UIPluginDataManager();

// AActor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

protected:

	virtual void BeginPlay() override;

// ARZ_UIPluginDataManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TMap<FName, TSubclassOf<class UUserWidget>> MenuWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TMap<FName, TSubclassOf<class UUserWidget>> HUDWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_MenuLayout_WB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_HUDLayout_WB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_Exit_WB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_Button_Small_WB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_Button_Large_WB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_DamageTakenNotification_WB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_UIPluginDataManager")
	TSubclassOf<class UUserWidget> RZ_DamageDealtNotification_WB;
};
