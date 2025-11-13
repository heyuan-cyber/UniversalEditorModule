// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SNEditorSetting.generated.h"

/**
 *
 */
UCLASS(config=Editor, defaultconfig)
class SCENARIONARRATIVEEDITOR_API USNEditorSetting : public UObject
{
    GENERATED_BODY()
public:
    // 帧率，保存到 DefaultEditor.ini
    UPROPERTY(config, EditAnywhere, Category="SNEditor")
    uint32 FrameRate = 60;

    UPROPERTY(config, EditAnywhere, Category = "SNEditor")
    uint32 MAX_FRAME_COUNT = 100000;
};
