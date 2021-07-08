#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_UIPluginInterfaces.generated.h"

UINTERFACE(MinimalAPI)
class URZ_BaseMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_UIPLUGIN_API IRZ_BaseMenuInterface
{
	GENERATED_BODY()

public:

	// Called from wherever the widget has been added/removed (WidgetSwitcher for ex.).
	// A small hack because I can't find an engine callback event.

	virtual void AddedToViewportCallback() = 0;
	virtual void RemovedFromViewportCallback() = 0;
};
