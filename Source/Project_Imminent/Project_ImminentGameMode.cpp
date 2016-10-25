// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Project_Imminent.h"
#include "Project_ImminentGameMode.h"
#include "Project_ImminentHUD.h"
#include "Project_ImminentCharacter.h"

AProject_ImminentGameMode::AProject_ImminentGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProject_ImminentHUD::StaticClass();
}
