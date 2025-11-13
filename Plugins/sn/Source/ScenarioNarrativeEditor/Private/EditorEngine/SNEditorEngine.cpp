#include "EditorEngine/SNEditorEngine.h"
#include "ActorGroupingUtils.h"
#include "ComponentVisualizerManager.h"
#include "EditorModeManager.h"
#include "InputState.h"
#include "SplineComponentVisualizer.h"
#include "UnrealWidget.h"
#include "Components/SplineComponent.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "Editor/GroupActor.h"
#include "EditorEngine/Elements/Actor/ActorElementEditorSelectionCustomization.h"
#include "EditorEngine/Elements/Actor/ActorElementSNEditorViewportInteractionCustomization.h"
#include "EditorEngine/Elements/Component/ComponentElementEditorSelectionCustomization.h"
#include "Elements/Framework/TypedElementSelectionSet.h"
#include "Elements/Interfaces/TypedElementObjectInterface.h"
#include "Elements/Interfaces/TypedElementSelectionInterface.h"
#include "Elements/Interfaces/TypedElementWorldInterface.h"
#include "Elements/Framework/TypedElementList.h"
#include "Elements/Framework/TypedElementRegistry.h"
#include "Elements/Framework/TypedElementViewportInteraction.h"
#include "Elements/Interfaces/TypedElementHierarchyInterface.h"
#include "PreviewScene/Viewport/SNAssetViewportClient.h"
#include "ScenarioNarrativeSequencer/SNSequencerEditorToolkit.h"

USNEditorEngine* USNEditorEngine::GMyEditorEngine = nullptr;

USNEditorEngine::USNEditorEngine():
    CachedElementsToManipulate(UTypedElementRegistry::GetInstance()->CreateElementList())
{
}

USNEditorEngine::~USNEditorEngine()
{
}

void USNEditorEngine::Init()
{
    RegisterComponentVisualizer();
    InitializeSelectionCustomization();
    InitializeInteractionCustomization();
}

void USNEditorEngine::InitializeSelectionCustomization()
{
    SNEditorElementSelectionSet = NewObject<UTypedElementSelectionSet>(GetTransientPackage(), NAME_None, RF_Transactional);
    SNEditorElementSelectionSet->AddToRoot();
    SNEditorElementSelectionSet->OnChanged().AddLambda([this](const UTypedElementSelectionSet* SelectionSet)
    {
       this->OnSelectedElementChanged(SelectionSet);
    });
    SNEditorElementSelectionSet->OnChanged().AddRaw(this->SNEditor, &FSNSequencerEditorToolkit::OnElementSelectionChanged);

    // Register the level editor specific selection behavior
    {
        TUniquePtr<FActorElementEditorSelectionCustomization> ActorCustomization = MakeUnique<FActorElementEditorSelectionCustomization>();
        ActorCustomization->SetToolkitHost(ToolkitHost);
        SNEditorElementSelectionSet->RegisterInterfaceCustomizationByTypeName(NAME_Actor, MoveTemp(ActorCustomization));
    }
    {
        TUniquePtr<FComponentElementEditorSelectionCustomization> ComponentCustomization = MakeUnique<FComponentElementEditorSelectionCustomization>();
        ComponentCustomization->SetToolkitHost(ToolkitHost);
        SNEditorElementSelectionSet->RegisterInterfaceCustomizationByTypeName(NAME_Components, MoveTemp(ComponentCustomization));
    }
    {

    }
}

void USNEditorEngine::InitializeInteractionCustomization()
{
    // 暂时用不到自定义的功能，默认即可
    FSNAssetViewportClient* SNViewportClient = GetSNViewportClient();
    {
        SNViewportClient->GetViewportInteraction()->GetDefaultInterfaceCustomization()->SetToolkitHost(ToolkitHost);
    }

    {
        TUniquePtr<FActorElementSNEditorViewportInteractionCustomization> ActorCustomization = MakeUnique<FActorElementSNEditorViewportInteractionCustomization>();
        ActorCustomization->SetToolkitHost(ToolkitHost);
        SNViewportClient->GetViewportInteraction()->RegisterInterfaceCustomizationByTypeName(NAME_Actor, MoveTemp(ActorCustomization));
    }
}


void USNEditorEngine::Destory()
{
    UnRegisterComponentVisualizer();
    SNEditorElementSelectionSet->OnChanged().Clear();
    SNEditorElementSelectionSet->RemoveFromRoot();
    SNEditorElementSelectionSet->MarkAsGarbage();
    SNEditorElementSelectionSet = nullptr;
    //CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

FSNAssetViewportClient* USNEditorEngine::GetSNViewportClient()
{
    return static_cast<FSNAssetViewportClient*>(ViewportClient);
}

#pragma region SelectElement

void USNEditorEngine::OnSelectedElementChanged(const UTypedElementSelectionSet* SelectionSet)
{
    VisualizersForSelection.Empty();

    auto GetVisualizersForSelection = [this](AActor* Actor, const UActorComponent* SelectedComponent)
    {
        // Iterate over components of that actor (and recurse through child components)
        TInlineComponentArray<UActorComponent*> Components;
        Actor->GetComponents(Components, true);

        for (int32 CompIdx = 0; CompIdx < Components.Num(); CompIdx++)
        {
            TWeakObjectPtr<UActorComponent> Comp(Components[CompIdx]);
            if (Comp.IsValid() && Comp->IsRegistered())
            {
                // Try and find a visualizer
                TSharedPtr<FComponentVisualizer> Visualizer = FindComponentVisualizer(Comp->GetClass()->GetFName());
                if (Visualizer.IsValid())
                {
                    FCachedComponentVisualizer CachedComponentVisualizer(Comp.Get(), Visualizer);
                    FComponentVisualizerForSelection Temp{ CachedComponentVisualizer };

                    FComponentVisualizerForSelection& ComponentVisualizerForSelection = VisualizersForSelection.Add_GetRef(MoveTemp(Temp));

                    ComponentVisualizerForSelection.IsEnabledDelegate.Emplace([bIsSelectedComponent = (Comp == SelectedComponent), WeakViz = TWeakPtr<FComponentVisualizer>(Visualizer), Comp]()
                    {
                        if (bIsSelectedComponent || (GetDefault<UEditorPerProjectUserSettings>()->bShowSelectionSubcomponents == true))
                        {
                            return true;
                        }

                        bool bShouldShowWithSelection = true;
                        if (TSharedPtr<FComponentVisualizer> PinnedViz = WeakViz.Pin())
                        {
                            bShouldShowWithSelection = PinnedViz->ShouldShowForSelectedSubcomponents(Comp.Get());
                        }
                        return bShouldShowWithSelection;
                    });
                }
            }
        }
    };

    UTypedElementSelectionSet* EditorSelectionSet = SNEditorElementSelectionSet;
    TSet<AActor*> ActorsProcessed;
    EditorSelectionSet->ForEachSelectedObject<UActorComponent>([&ActorsProcessed, &GetVisualizersForSelection](UActorComponent* InActorComponent)
        {
            if (AActor* Actor = InActorComponent->GetOwner())
            {
                if (!ActorsProcessed.Contains(Actor))
                {
                    GetVisualizersForSelection(Actor, InActorComponent);
                    ActorsProcessed.Emplace(Actor);
                }
            }
            return true;
        });

    if (ActorsProcessed.Num() == 0)
    {
        EditorSelectionSet->ForEachSelectedObject<AActor>([&GetVisualizersForSelection](AActor* InActor)
            {
                GetVisualizersForSelection(InActor, InActor->GetRootComponent());
                return true;
            });
    }

    // Restore the active component visualizer, since an undo/redo may have changed the selection
    bool bDidSetVizThisTick = false;
    if (VisualizersForSelection.Num() > 0)
    {
        for (FComponentVisualizerForSelection& VisualizerForSelection : VisualizersForSelection)
        {
            if (!CVM->IsActive() && VisualizerForSelection.ComponentVisualizer.Visualizer->GetEditedComponent() != nullptr)
            {
                CVM->SetActiveComponentVis(ViewportClient, VisualizerForSelection.ComponentVisualizer.Visualizer);
                bDidSetVizThisTick = true;
                break;
            }

            if (CVM->IsActive() && VisualizerForSelection.ComponentVisualizer.Visualizer->GetEditedComponent() == CVM->GetActiveComponentVis()->GetEditedComponent())
            {
                bDidSetVizThisTick = true;
                break;
            }
        }
    }

    if (CVM->IsActive() && (VisualizersForSelection.Num() == 0 || !bDidSetVizThisTick))
    {
        CVM->ClearActiveComponentVis();
    }

    NoteSelectionChange(true);
}

void USNEditorEngine::NoteSelectionChange(bool bNotify)
{
    // The selection changed, so make sure the pivot (widget) is located in the right place
    UpdatePivotLocationForSelection( true );

    UpdateFloatingPropertyWindows(/*bForceRefresh*/false, true);
}

void USNEditorEngine::UpdateFloatingPropertyWindows(bool bForceRefresh, bool bNotifyActorSelectionChanged)
{
    if (const UTypedElementSelectionSet* SelectionSet = SNEditorElementSelectionSet)
    {


    }
}

bool USNEditorEngine::ClickElement(FEditorViewportClient* InViewportClient, const FTypedElementHandle& HitElement, const FViewportClick& Click)
{
    bool bHandledClick = false;
    const bool bIsLeftClickSelection = Click.GetKey() == EKeys::LeftMouseButton && !(InViewportClient->Viewport->KeyState(EKeys::T) || InViewportClient->Viewport->KeyState(EKeys::L) || InViewportClient->Viewport->KeyState(EKeys::S) || InViewportClient->Viewport->KeyState(EKeys::A));
    const bool bIsRightClickSelection = Click.GetKey() == EKeys::RightMouseButton && !Click.IsControlDown() && !InViewportClient->Viewport->KeyState(EKeys::LeftMouseButton);

if (bIsLeftClickSelection || bIsRightClickSelection)

{
    const ETypedElementSelectionMethod SelectionMethod = Click.GetEvent() == IE_DoubleClick ? ETypedElementSelectionMethod::Secondary : ETypedElementSelectionMethod::Primary;

    if (const FTypedElementHandle& ResolvedElement = SNEditorElementSelectionSet->GetSelectionElement(HitElement, SelectionMethod))
    {
        bHandledClick = true;

        const FTypedElementSelectionOptions SelectionOptions = FTypedElementSelectionOptions()
            .SetAllowHidden(true)
            .SetWarnIfLocked(true);

        bool bNeedViewportRefresh = false;

        if (SNEditorElementSelectionSet->CanSelectElement(ResolvedElement, SelectionOptions))
        {
            const FScopedTransaction Transaction(NSLOCTEXT("SNEditor", "ClickingOnElements", "Clicking on Elements"));

            const bool bAllowSelectionModifiers = bIsLeftClickSelection && SNEditorElementSelectionSet->AllowSelectionModifiers(ResolvedElement);
            if (Click.IsControlDown() && bAllowSelectionModifiers)
            {
                if (SNEditorElementSelectionSet->IsElementSelected(ResolvedElement, FTypedElementIsSelectedOptions().SetAllowIndirect(true)))
                {
                    SNEditorElementSelectionSet->DeselectElement(ResolvedElement, SelectionOptions);
                }
                else
                {
                    SNEditorElementSelectionSet->SelectElement(ResolvedElement, SelectionOptions);
                }
            }
            else if (Click.IsShiftDown() && bAllowSelectionModifiers)
            {
                SNEditorElementSelectionSet->SelectElement(ResolvedElement, SelectionOptions);
            }
            else
            {
                // Skip the clear if we're doing a RMB select and this actor is already selected, as we want to summon the menu for the current selection
                if (bIsLeftClickSelection || !SNEditorElementSelectionSet->IsElementSelected(ResolvedElement, FTypedElementIsSelectedOptions().SetAllowIndirect(true)))
                {
                    bNeedViewportRefresh = bIsRightClickSelection; // Refresh the viewport so the user will see what they just clicked while the menu is open
                    GEditor->DeselectAllSurfaces();
                    SNEditorElementSelectionSet->ClearSelection(SelectionOptions);
                }
                SNEditorElementSelectionSet->SelectElement(ResolvedElement, SelectionOptions);
            }

            // Notify any pending selection change now, as this avoids the visual pivot location "lagging" behind the actual selection,
            // and also ensures that the pivot is at the correct location prior to opening any context menus (which block the update)
            SNEditorElementSelectionSet->NotifyPendingChanges();
        }

        if (bNeedViewportRefresh)
        {
            // Redraw the viewport so the user can see which object was clicked on
            InViewportClient->Viewport->Draw();
            FlushRenderingCommands();
        }

        if (bIsRightClickSelection)
        {
            //PrivateSummonContextMenu(ViewportClient, ResolvedElement);
        }
    }
}

return bHandledClick;
}

bool USNEditorEngine::ClickViewport(FEditorViewportClient* InViewportClient, const FViewportClick& Click)
{
    return false;
}

bool USNEditorEngine::ClickActor(FEditorViewportClient* InViewportClient, AActor* Actor, const FViewportClick& Click, bool bAllowSelectionChange)
{
    return false;
}

bool USNEditorEngine::ClickComponent(FEditorViewportClient* InViewportClient, HActor* ActorHitProxy, const FViewportClick& Click)
{
    return false;
}

void USNEditorEngine::SelectNone(bool bNoteSelectionChange, bool bDeselectBSPSurfs, bool WarnAboutManyActors)
{
    bool bSelectionChanged = false;

    UTypedElementSelectionSet* SelectionSet = SNEditorElementSelectionSet;

    FTypedElementList::FScopedClearNewPendingChange ClearNewPendingChange;

    if (SelectionSet)
    {
        if (!bNoteSelectionChange)
        {
            ClearNewPendingChange = SelectionSet->GetScopedClearNewPendingChange();
        }
        bSelectionChanged |= SelectionSet->ClearSelection(FTypedElementSelectionOptions().SetAllowLegacyNotifications(false));
    }

    if (bSelectionChanged)
    {
        if (SelectionSet)
        {
            if (bNoteSelectionChange)
            {
                SelectionSet->NotifyPendingChanges();
            }
        }
    }
}

void USNEditorEngine::ClickBackdrop(FEditorViewportClient* InViewportClient,const FViewportClick& Click)
{
    if( Click.GetKey() == EKeys::LeftMouseButton )
    {
        if( !Click.IsControlDown() )
        {
            const FScopedTransaction Transaction( NSLOCTEXT("SNEditor", "ClickingBackground", "Clicking Background") );
            SelectNone( true, true );
        }
    }
}
#pragma endregion

#pragma region SelectedElement

FTypedElementListRef USNEditorEngine::GetElementsToManipulate(const bool bForceRefresh)
{
    CacheElementsToManipulate(bForceRefresh);
    return CachedElementsToManipulate.ToSharedRef();
}

void USNEditorEngine::CacheElementsToManipulate(const bool bForceRefresh)
{
    if (bForceRefresh)
    {
        ResetElementsToManipulate();
    }

    if (!bHasCachedElementsToManipulate)
    {
        const FTypedElementSelectionNormalizationOptions NormalizationOptions = FTypedElementSelectionNormalizationOptions()
            .SetExpandGroups(true)
            .SetFollowAttachment(true);

        const UTypedElementSelectionSet* SelectionSet = SNEditorElementSelectionSet;
        SelectionSet->GetNormalizedSelection(NormalizationOptions, CachedElementsToManipulate.ToSharedRef());

        const UE::Widget::EWidgetMode WidgetMode = GetWidgetMode();

        // Remove any elements that cannot be moved
        CachedElementsToManipulate->RemoveAll<ITypedElementWorldInterface>([this, WidgetMode](const TTypedElement<ITypedElementWorldInterface>& InWorldElement)
        {
            if (!InWorldElement.CanMoveElement(bIsSimulateInEditorViewport ? ETypedElementWorldType::Game : ETypedElementWorldType::Editor))
            {
                return true;
            }

            if (WidgetMode == UE::Widget::WM_Scale && !InWorldElement.CanScaleElement())
            {
                return true;
            }

            return false;
        });

        bHasCachedElementsToManipulate = true;
    }
}

void USNEditorEngine::ResetElementsToManipulate(const bool bClearList)
{
    if (bClearList)
    {
        CachedElementsToManipulate->Reset();
    }
    bHasCachedElementsToManipulate = false;
}

AActor* USNEditorEngine::GetSelectedActor()
{
    AActor* Selected = nullptr;

    if (UTypedElementSelectionSet* SelectionSet = SNEditorElementSelectionSet)
    {
        SelectionSet->ForEachSelectedElement<ITypedElementObjectInterface>(
            [&](const TTypedElement<ITypedElementObjectInterface>& ObjectElement)
            {
                if (UObject* Obj = ObjectElement.GetObject())
                {
                    if (AActor* Actor = Cast<AActor>(Obj))
                    {
                        Selected = Actor;
                    }
                    else if (UActorComponent* Comp = Cast<UActorComponent>(Obj))
                    {
                        if (AActor* Owner = Comp->GetOwner())
                        {
                            Selected = Owner;
                        }
                    }
                }
                return true; // 继续遍历
            });
    }
    return Selected;
}

UActorComponent* USNEditorEngine::GetSelectedActorComponent()
{
    UActorComponent* Selected = nullptr;

    if (UTypedElementSelectionSet* SelectionSet = SNEditorElementSelectionSet)
    {
        SelectionSet->ForEachSelectedElement<ITypedElementObjectInterface>(
            [&](const TTypedElement<ITypedElementObjectInterface>& ObjectElement)
            {
                if (UObject* Obj = ObjectElement.GetObject())
                {
                    if (UActorComponent* Comp = Cast<UActorComponent>(Obj))
                    {
                        Selected = Comp;
                    }
                }
                return true; // 继续遍历
            });
    }
    return Selected;
}
#pragma endregion

#pragma region ComponentVisualizer
void USNEditorEngine::RegisterComponentVisualizer(FName ComponentClassName, TSharedPtr<FComponentVisualizer> Visualizer)
{
    if( !ComponentClassName.IsNone() )
    {
        ComponentVisualizerMap.Add(ComponentClassName, Visualizer);
    }

    RegisteredComponentClassNames.Add(ComponentClassName);

    if (Visualizer.IsValid())
    {
        Visualizer->OnRegister();
    }
}

void USNEditorEngine::RegisterComponentVisualizer()
{
    if (!CVM)
    {
        CVM = MakeShared<FComponentVisualizerManager>();
        RegisterComponentVisualizer(USplineComponent::StaticClass()->GetFName(), MakeShareable(new FSplineComponentVisualizer));
    }
}

void USNEditorEngine::UnRegisterComponentVisualizer()
{
    for(FName ClassName : RegisteredComponentClassNames)
    {
        UnregisterComponentVisualizer(ClassName);
    }
}

void USNEditorEngine::UnregisterComponentVisualizer(FName ComponentClassName)
{
    if (TSharedPtr<FComponentVisualizer> Visualizer = FindComponentVisualizer(ComponentClassName))
    {
        ComponentVisualizerMap.Remove(ComponentClassName);
    }
}

TSharedPtr<FComponentVisualizer> USNEditorEngine::FindComponentVisualizer(FName ComponentClassName) const
{
    TSharedPtr<FComponentVisualizer> Visualizer = NULL;

    const TSharedPtr<FComponentVisualizer>* VisualizerPtr = ComponentVisualizerMap.Find(ComponentClassName);
    if(VisualizerPtr != NULL)
    {
        Visualizer = *VisualizerPtr;
    }

    return Visualizer;
}

void USNEditorEngine::SetActiveComponentVisualizar(FEditorViewportClient* InViewportClient,UActorComponent* ComClass)
{
    TSharedPtr<FComponentVisualizer> Visualizer = FindComponentVisualizer(ComClass->GetClass()->GetFName());
    CVM->ClearActiveComponentVis();
    CVM->SetActiveComponentVis(InViewportClient, Visualizer);
}

void USNEditorEngine::DrawComponentVisualizers(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    for(FComponentVisualizerForSelection& VisualizerForSelection : VisualizersForSelection)
    {
        if (VisualizerForSelection.IsEnabledDelegate)
        {
            if (VisualizerForSelection.ComponentVisualizer.Visualizer)
            {
                VisualizerForSelection.ComponentVisualizer.Visualizer->DrawVisualization(VisualizerForSelection.ComponentVisualizer.ComponentPropertyPath.GetComponent(), View, PDI);
            }
        }
    }
}
#pragma endregion

#pragma region GizmoWidget
UE::Widget::EWidgetMode USNEditorEngine::GetWidgetMode() const
{
    if (CVM->IsActive() && CVM->IsVisualizingArchetype())
    {
        return UE::Widget::WM_None;
    }

    return ViewportClient->GetWidgetMode();
}

// 只有点击切换时才会触发，只处理Actor层级(不管 Component Visualizer 的点击)
void USNEditorEngine::UpdatePivotLocationForSelection( bool bOnChange )
{
    if (!ViewportClient)
    {
        return;
    }

    // Pick a new common pivot, or not.
    TTypedElement<ITypedElementWorldInterface> SingleWorldElement;

    FTypedElementListConstPtr ElementsToManipulate = GetElementsToManipulate(true);
    ElementsToManipulate->ForEachElement<ITypedElementWorldInterface>([&SingleWorldElement](const TTypedElement<ITypedElementWorldInterface>& InWorldElement)
    {
#if DO_CHECK
        if (ULevel* OwnerLevel = InWorldElement.GetOwnerLevel())
        {
            const bool bIsTemplate = InWorldElement.IsTemplateElement();
        }
#endif // DO_CHECK

        SingleWorldElement = InWorldElement;
        return true;
    });

    if (SingleWorldElement.IsSet())
    {
        if (bOnChange)
        {
            FTransform ElementWorldTransform;
            SingleWorldElement.GetWorldTransform(ElementWorldTransform);

            FVector ElementPivotOffset = FVector::ZeroVector;
            SingleWorldElement.GetPivotOffset(ElementPivotOffset);

            // Set pivot point to the element location, accounting for any set pivot offset
            FVector PivotPoint = ElementWorldTransform.TransformPosition(ElementPivotOffset);

            SetPivot(PivotPoint, false, true);
        }
    }
    else
    {
        ResetPivot();
    }

    //SetPivotMovedIndependently(false);
}

// Gizmo Update Actor Transform
void USNEditorEngine::ApplyDeltaToSelectedElements(const FTransform& InDeltaTransform)
{
    FScopedTransaction Transaction(NSLOCTEXT("SNEditor", "Move", "Move"));
    if (InDeltaTransform.GetTranslation().IsZero() && InDeltaTransform.Rotator().IsZero() && InDeltaTransform.GetScale3D().IsZero())
    {
        return;
    }

    FTransform ModifiedDeltaTransform = InDeltaTransform;

    {
        FVector AdjustedScale = ModifiedDeltaTransform.GetScale3D();

        // If we are scaling, we need to change the scaling factor a bit to properly align to grid
        //if (GEditor->UsePercentageBasedScaling() && !AdjustedScale.IsNearlyZero())
        //{
        //    AdjustedScale *= ((GEditor->GetScaleGridSize() / 100.0f) / GEditor->GetGridSize());
        //}

        ModifiedDeltaTransform.SetScale3D(AdjustedScale);
    }

    FInputDeviceState InputState;
    InputState.SetModifierKeyStates(ViewportClient->IsShiftPressed(), ViewportClient->IsAltPressed(), ViewportClient->IsCtrlPressed(), ViewportClient->IsCmdPressed());

    FTypedElementListConstRef ElementsToManipulate = GetElementsToManipulate();
    GetSNViewportClient()->GetViewportInteraction()->UpdateGizmoManipulation(ElementsToManipulate, GetWidgetMode(), GetSNViewportClient()->GetWidget() ? GetSNViewportClient()->GetWidget()->GetCurrentAxis() : EAxisList::None, InputState, ModifiedDeltaTransform);
}

// 只有点击切换时才会触发，只处理Actor层级
void USNEditorEngine::SetPivot( FVector NewPivot, bool bSnapPivotToGrid, bool bIgnoreAxis, bool bAssignPivot/*=false*/ )
{
    if (!ViewportClient)
    {
        return;
    }

    FEditorModeTools* EditorModeTools = ViewportClient->GetModeTools();
    USelection* Actors = EditorModeTools->GetSelectedActors();
    USelection* Objects = EditorModeTools->GetSelectedObjects();
    USelection* Coms = EditorModeTools->GetSelectedComponents();

    if( !bIgnoreAxis )
    {
        // Don't stomp on orthonormal axis.
        // TODO: this breaks if there is genuinely a need to set the pivot to a coordinate containing a zero component
         if( NewPivot.X==0 ) NewPivot.X=EditorModeTools->PivotLocation.X;
         if( NewPivot.Y==0 ) NewPivot.Y=EditorModeTools->PivotLocation.Y;
         if( NewPivot.Z==0 ) NewPivot.Z=EditorModeTools->PivotLocation.Z;
    }

    // Set the pivot.
    EditorModeTools->SetPivotLocation(NewPivot, false);
    EditorModeTools->SetWidgetMode(UE::Widget::WM_Translate);
    if( bSnapPivotToGrid )
    {
        FRotator DummyRotator(0,0,0);
        EditorModeTools->PivotLocation = EditorModeTools->SnappedLocation;
    }

    // Check all elements.
    int32 NumElements = 0;

    //default to using the x axis for the translate rotate widget
    EditorModeTools->TranslateRotateXAxisAngle = 0.0f;
    EditorModeTools->TranslateRotate2DAngle = 0.0f;

    FVector TranslateRotateWidgetWorldXAxis = FVector::ZeroVector;
    FVector Widget2DWorldXAxis = FVector::ZeroVector;

    // Pick a new common pivot, or not.
    TTypedElement<ITypedElementWorldInterface> SingleWorldElement;

    FTypedElementListConstPtr ElementsToManipulate = GetElementsToManipulate(true);
    ElementsToManipulate->ForEachElement<ITypedElementWorldInterface>([&NumElements, &TranslateRotateWidgetWorldXAxis, &Widget2DWorldXAxis, &SingleWorldElement](const TTypedElement<ITypedElementWorldInterface>& InWorldElement)
    {
        if (NumElements == 0)
        {
            FTransform ElementWorldTransform;
            InWorldElement.GetWorldTransform(ElementWorldTransform);

            TranslateRotateWidgetWorldXAxis = ElementWorldTransform.TransformVector(FVector(1.0f, 0.0f, 0.0f));
            //get the xy plane project of this vector
            TranslateRotateWidgetWorldXAxis.Z = 0.0f;
            if (!TranslateRotateWidgetWorldXAxis.Normalize())
            {
                TranslateRotateWidgetWorldXAxis = FVector(1.0f, 0.0f, 0.0f);
            }

            Widget2DWorldXAxis = ElementWorldTransform.TransformVector(FVector(1, 0, 0));
            Widget2DWorldXAxis.Y = 0;
            if (!Widget2DWorldXAxis.Normalize())
            {
                Widget2DWorldXAxis = FVector(1, 0, 0);
            }
        }

        SingleWorldElement = InWorldElement;
        ++NumElements;
        return true;
    });

    if (bAssignPivot && UActorGroupingUtils::IsGroupingActive())
    {
        if (TTypedElement<ITypedElementObjectInterface> ObjectElement = ElementsToManipulate->GetElement<ITypedElementObjectInterface>(SingleWorldElement))
        {
            if (AActor* SingleActor = Cast<AActor>(ObjectElement.GetObject()))
            {
                // set group pivot for the root-most group
                if (AGroupActor* ActorGroupRoot = AGroupActor::GetRootForActor(SingleActor, true, true))
                {
                    ActorGroupRoot->SetActorLocation(EditorModeTools->PivotLocation, false);
                }
            }
        }
    }

    //if there are multiple elements selected, just use the x-axis for the "translate/rotate" or 2D widgets
    if (NumElements == 1)
    {
        EditorModeTools->TranslateRotateXAxisAngle = static_cast<float>(TranslateRotateWidgetWorldXAxis.Rotation().Yaw);
        EditorModeTools->TranslateRotate2DAngle = static_cast<float>(FMath::RadiansToDegrees(FMath::Atan2(Widget2DWorldXAxis.Z, Widget2DWorldXAxis.X)));
    }

    // Update showing.
    EditorModeTools->PivotShown = NumElements > 0;
}

// 隐藏Gizmo Widget
void USNEditorEngine::ResetPivot()
{
    FEditorModeTools* EditorModeTools = ViewportClient->GetModeTools();

    // 与Widget渲染逻辑直接相关
    EditorModeTools->SetWidgetMode(UE::Widget::WM_None);
}
#pragma endregion

#undef LOCTEXT_NAMESPACE
