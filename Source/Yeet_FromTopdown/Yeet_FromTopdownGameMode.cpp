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

	static ConstructorHelpers::FClassFinder<AActor> GridBPClass(TEXT("/Game/blueprint/Grid_bp"));
	if (GridBPClass.Class != NULL)
	{
		GridClass = GridBPClass.Class;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Grid Class LOADED!!!!"));
	}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Grid Class NOT LOADED!!!!"));
	//}

}