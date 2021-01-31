// Copyright kevin791129

#include "ColorPicker.h"

#define LOCTEXT_NAMESPACE "FColorPickerModule"

DEFINE_LOG_CATEGORY(LogColorPicker);
DEFINE_LOG_CATEGORY(LogColorPickerWarning);
DEFINE_LOG_CATEGORY(LogColorPickerError);

void FColorPickerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FColorPickerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FColorPickerModule, ColorPicker)