#pragma once
#include "Tools/LegacyEdModeWidgetHelpers.h"

/**
 编辑器框架类，用于自定义Gizmo Widget绘制条件
 */
class FSNEditorWidgetHelper : public FLegacyEdModeWidgetHelper
{
public:

    virtual bool ShouldDrawWidget() const override;
};
