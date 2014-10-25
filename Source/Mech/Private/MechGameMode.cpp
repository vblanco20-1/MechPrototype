// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Mech.h"
#include "MechGameMode.h"
#include "MechHUD.h"
#include "MechCharacter.h"
#include "MechPlayerController.h"

AMechGameMode::AMechGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	PlayerControllerClass = AMechPlayerController::StaticClass();
	// use our custom HUD class
	HUDClass = AMechHUD::StaticClass();
}
