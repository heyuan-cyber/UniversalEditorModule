// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiPlayerSystemGameMode.h"
#include "MultiPlayerSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiPlayerSystemGameMode::AMultiPlayerSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
