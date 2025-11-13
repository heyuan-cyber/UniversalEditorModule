// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tools/LegacyEdModeWidgetHelpers.h"
#include "SNEditorMode.generated.h"

/**
 编辑器框架类，用于自定义Gizmo Widget行为。如不指定，默认使用LevelEditorMode
 */
UCLASS()
class SCENARIONARRATIVEEDITOR_API USNEditorMode : public UBaseLegacyWidgetEdMode
{
    GENERATED_BODY()

public:
    static const FEditorModeID EdModeId;

    USNEditorMode();

    virtual ~USNEditorMode();

    virtual TSharedRef<FLegacyEdModeWidgetHelper> CreateWidgetHelper() override;

    virtual bool UsesToolkits() const override;
    bool UsesPropertyWidgets() const;
};
