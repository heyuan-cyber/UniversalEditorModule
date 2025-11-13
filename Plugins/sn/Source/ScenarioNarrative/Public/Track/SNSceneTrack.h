// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNTrackBase.h"
#include "SNSceneTrack.generated.h"

/**
 *
 */
UCLASS()
class SCENARIONARRATIVE_API USNSceneTrack : public USNTrackBase
{
    GENERATED_BODY()

#pragma region Properties
public:

    UPROPERTY(EditAnywhere, Category = "Level Config")
    TSoftObjectPtr<UWorld> LevelClass;

#pragma endregion

};
