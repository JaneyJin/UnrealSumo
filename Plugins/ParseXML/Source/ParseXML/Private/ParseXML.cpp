// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ParseXML.h"
#include "ParseXMLStyle.h"
#include "ParseXMLCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "Misc/MessageDialog.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "HAL/FileManager.h"
#include "fileParser.h"


static const FName ParseXMLTabName("ParseXML");

#define LOCTEXT_NAMESPACE "FParseXMLModule"

void FParseXMLModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FParseXMLStyle::Initialize();
	FParseXMLStyle::ReloadTextures();

	FParseXMLCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FParseXMLCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FParseXMLModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FParseXMLModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FParseXMLModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	/*FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ParseXMLTabName, FOnSpawnTab::CreateRaw(this, &FParseXMLModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FParseXMLTabTitle", "ParseXML"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);*/
}

void FParseXMLModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FParseXMLStyle::Shutdown();

	FParseXMLCommands::Unregister();

	// FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ParseXMLTabName);
}



void FParseXMLModule::PluginButtonClicked()
{
	// FGlobalTabmanager::Get()->InvokeTab(ParseXMLTabName);
	/*FText DialogText = FText::Format(
		LOCTEXT("Open your xml", "Message box"),
		FText::FromString(TEXT("FParseXMLModule::PluginButtonClicked()")),
		FText::FromString(TEXT("ParseXML.cpp"))
	);

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);*/


	const FString& windowTitle = "Open XML Files";
	FString RelativePath = FPaths::GameContentDir();
	FString FullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*RelativePath);
	const FString& defaultFilePath = FullPath;
	const FString& defaultFileName = "";
	const FString& defaultFileType = "*.xml";

	TArray <FString> originalOutFileNames = { };
	TArray < FString > & OutFilenames = originalOutFileNames;

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	const void* ParentWindowWindowHandle = nullptr;
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	const TSharedPtr<SWindow>& MainFrameParentWindow = MainFrameModule.GetParentWindow();
	if (MainFrameParentWindow.IsValid() && MainFrameParentWindow->GetNativeWindow().IsValid())
	{
		ParentWindowWindowHandle = MainFrameParentWindow->GetNativeWindow()->GetOSWindowHandle();
		if (DesktopPlatform->OpenFileDialog(ParentWindowWindowHandle, windowTitle, defaultFilePath, defaultFileName,
			defaultFileType, 0x00, OutFilenames)) {
			// GEngine->Exec(nullptr, TEXT("Log LogTemp off"));

			FString selectedFile = FString(OutFilenames[0]);
			// UE_LOG(LogTemp, Warning, TEXT("selected file %s"), *selectedFile)

			UfileParser fileParser(*selectedFile);
			fileParser.loadxml();
		}
		UE_LOG(LogTemp, Warning, TEXT("Plugin works!"));
	}


	
}

void FParseXMLModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FParseXMLCommands::Get().PluginAction);
}

void FParseXMLModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FParseXMLCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FParseXMLModule, ParseXML)