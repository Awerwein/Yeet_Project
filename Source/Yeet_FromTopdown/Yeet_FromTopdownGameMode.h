// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Yeet_FromTopdownGameMode.generated.h"

UCLASS(minimalapi)
class AYeet_FromTopdownGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AYeet_FromTopdownGameMode();

	UFUNCTION(BlueprintCallable)
		TArray<AActor*> GetGrid();

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<AActor> GridClass;


};



