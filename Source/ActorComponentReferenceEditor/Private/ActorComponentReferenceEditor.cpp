// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActorComponentReferenceEditor.h"

#include <ModuleManager.h>
#include <PropertyEditorModule.h>

#include <ActorComponentReferenceCustomization.h>

#define LOCTEXT_NAMESPACE "ActorComponentReferenceEditorModule"

void FActorComponentReferenceEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	PropertyModule.RegisterCustomPropertyTypeLayout("ActorComponentReference", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FActorComponentReferenceCustomization::MakeInstance));
}

void FActorComponentReferenceEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("ActorComponentReference");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FActorComponentReferenceEditorModule, ActorComponentReferenceEditor)