#pragma once
#include "Elements/Actor/ActorElementEditorViewportInteractionCustomization.h"

/*
 * 为了使用PostEditorMove接口
 */
class SCENARIONARRATIVEEDITOR_API FActorElementSNEditorViewportInteractionCustomization : public FActorElementEditorViewportInteractionCustomization
{
public:
    virtual void PostGizmoManipulationStopped(TArrayView<const FTypedElementHandle> InElementHandles, const UE::Widget::EWidgetMode InWidgetMode) override;

    virtual void GizmoManipulationDeltaUpdate(const TTypedElement<ITypedElementWorldInterface>& InElementWorldHandle, const UE::Widget::EWidgetMode InWidgetMode, const EAxisList::Type InDragAxis, const FInputDeviceState& InInputState, const FTransform& InDeltaTransform, const FVector& InPivotLocation) override;
};
