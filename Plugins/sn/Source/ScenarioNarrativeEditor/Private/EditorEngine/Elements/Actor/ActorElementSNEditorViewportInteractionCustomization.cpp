#include "EditorEngine/Elements/Actor/ActorElementSNEditorViewportInteractionCustomization.h"
#include "Elements/Actor/ActorElementData.h"

void FActorElementSNEditorViewportInteractionCustomization::PostGizmoManipulationStopped(
    TArrayView<const FTypedElementHandle> InElementHandles, const UE::Widget::EWidgetMode InWidgetMode)
{

}

void FActorElementSNEditorViewportInteractionCustomization::GizmoManipulationDeltaUpdate(const TTypedElement<ITypedElementWorldInterface>& InElementWorldHandle, const UE::Widget::EWidgetMode InWidgetMode, const EAxisList::Type InDragAxis, const FInputDeviceState& InInputState, const FTransform& InDeltaTransform, const FVector& InPivotLocation)
{
    FTypedElementViewportInteractionCustomization::GizmoManipulationDeltaUpdate(InElementWorldHandle, InWidgetMode, InDragAxis, InInputState, InDeltaTransform, InPivotLocation);
}
