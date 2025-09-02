// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PTPGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class APTPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	APTPGameMode();
};



