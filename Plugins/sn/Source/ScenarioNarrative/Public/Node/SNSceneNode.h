// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNNodeBase.h"
#include "SNSceneNode.generated.h"

/**
 *
 */
UCLASS()
class SCENARIONARRATIVE_API USNSceneNode : public USNNodeBase
{
    GENERATED_BODY()

#pragma region Properties
public:

    UPROPERTY(EditAnywhere, Category = "Level Config")
    TSoftObjectPtr<UWorld> LevelClass;

#pragma endregion

# pragma region Function

    virtual TSet<UClass*> AttachedToTracks() const override;

#pragma endregion
};
