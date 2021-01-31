// Copyright kevin791129

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogColorPicker, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogColorPickerWarning, Warning, All);
DECLARE_LOG_CATEGORY_EXTERN(LogColorPickerError, Error, All);

class FColorPickerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
