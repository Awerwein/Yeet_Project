// Copyright Epic Games, Inc. All Rights Reserved.

#include "Yeet_FromTopdownGameMode.h"
#include "Yeet_FromTopdownPlayerController.h"
#include "Yeet_FromTopdownCharacter.h"
#include "UObject/ConstructorHelpers.h"

AYeet_FromTopdownGameMode::AYeet_FromTopdownGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AYeet_FromTopdownPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}