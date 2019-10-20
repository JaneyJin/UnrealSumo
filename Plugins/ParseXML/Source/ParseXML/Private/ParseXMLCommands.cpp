// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ParseXMLCommands.h"

#define LOCTEXT_NAMESPACE "FParseXMLModule"

void FParseXMLCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ParseXML", "Bring up ParseXML window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
