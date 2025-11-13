// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ComponentVisualizer.h"
#include "Elements/Framework/TypedElementListFwd.h"
#include "SNEditorEngine.generated.h"

struct HActor;
class FComponentVisualizer;
class FComponentVisualizerManager;
class FSNAssetViewportClient;
class FSNSequencerEditorToolkit;
class IToolkitHost;

/**
 编辑器管理类，用于物体选中、组件绘制、GizmoWidget、细节面板更新等关键逻辑
    -使用最新的TypedElementFramework架构
 */
UCLASS()
class SCENARIONARRATIVEEDITOR_API USNEditorEngine : public UObject
{
    GENERATED_BODY()
public:
    USNEditorEngine();

    virtual ~USNEditorEngine();

# pragma region Singleton
public:
    static USNEditorEngine* GMyEditorEngine;

    /** 获取全局实例 */
    static USNEditorEngine* Get()
    {
        check(GMyEditorEngine); // 确保已经初始化
        return GMyEditorEngine;
    }

    /** 初始化单例，传入 World */
    static void NewInstance()
    {
        if (!GMyEditorEngine)
        {
            GMyEditorEngine = NewObject<USNEditorEngine>(GetTransientPackage(), NAME_None, RF_Standalone | RF_Transient);
            GMyEditorEngine->AddToRoot(); // 防止 GC 回收
        }
    }

    /** 卸载 */
    static void Shutdown()
    {
        if (GMyEditorEngine)
        {
            GMyEditorEngine->RemoveFromRoot(); // 取消 GC Root
            GMyEditorEngine = nullptr;
        }
    }
#pragma endregion

#pragma region Properties
    struct FComponentVisualizerForSelection
    {
        FCachedComponentVisualizer ComponentVisualizer;
        TOptional<TFunction<bool(void)>> IsEnabledDelegate;
    };

    /** 用于视口中组件可视化管理，如绘制、点击 */
    TSharedPtr<FComponentVisualizerManager> CVM;

    /** Array of component class names we have registered, so we know what to unregister afterwards */
    TArray<FName> RegisteredComponentClassNames;

    /** Map from component class to visualizer object to use */
    TMap< FName, TSharedPtr<class FComponentVisualizer> > ComponentVisualizerMap;

    /** 选中Element容器，基于UE最新的通用TypedElement架构 */
    UTypedElementSelectionSet* SNEditorElementSelectionSet;

    TArray<FComponentVisualizerForSelection> VisualizersForSelection;

    FEditorViewportClient* ViewportClient;

    bool bHasCachedElementsToManipulate = false;

    FTypedElementListPtr CachedElementsToManipulate;

    bool bIsSimulateInEditorViewport = false;

    /** The toolkit host associated with our SNEditor. */
    IToolkitHost* ToolkitHost = nullptr;

    FSNSequencerEditorToolkit* SNEditor = nullptr;
#pragma endregion

#pragma region Functions - Common
    void Init();
    void InitializeSelectionCustomization();
    void InitializeInteractionCustomization();

    void Destory();
#pragma endregion

#pragma region Function - Utils
    FSNAssetViewportClient* GetSNViewportClient();

    AActor* GetSelectedActor();

    UActorComponent* GetSelectedActorComponent();

    UE::Widget::EWidgetMode GetWidgetMode() const;

    FTypedElementListRef GetElementsToManipulate(const bool bForceRefresh = false);
#pragma endregion

#pragma region Functions - Component Visualizer
    void RegisterComponentVisualizer(FName ComponentClassName, TSharedPtr<FComponentVisualizer> Visualizer);

    void RegisterComponentVisualizer();

    void UnRegisterComponentVisualizer();

    void UnregisterComponentVisualizer(FName ComponentClassName);

    TSharedPtr<class FComponentVisualizer> FindComponentVisualizer(FName ComponentClassName) const;

    void SetActiveComponentVisualizar(FEditorViewportClient* InViewportClient, UActorComponent* ComClass);

    void DrawComponentVisualizers(const FSceneView* View, FPrimitiveDrawInterface* PDI);
#pragma endregion

#pragma region Function - Select Element
    bool ClickElement(FEditorViewportClient* ViewportClient, const FTypedElementHandle& HitElement, const FViewportClick& Click);

    bool ClickViewport(FEditorViewportClient* ViewportClient, const FViewportClick& Click);

    bool ClickActor(FEditorViewportClient* ViewportClient, AActor* Actor, const FViewportClick& Click, bool bAllowSelectionChange);

    bool ClickComponent(FEditorViewportClient* ViewportClient, HActor* ActorHitProxy, const FViewportClick& Click);

    void ClickBackdrop(FEditorViewportClient* ViewportClient, const FViewportClick& Click);

    void SelectNone(bool bNoteSelectionChange, bool bDeselectBSPSurfs, bool WarnAboutManyActors = true);

    void OnSelectedElementChanged(const UTypedElementSelectionSet* SelectionSet);

    void NoteSelectionChange(bool bNotify);

    void UpdateFloatingPropertyWindows(bool bForceRefresh, bool bNotifyActorSelectionChanged);

    void CacheElementsToManipulate(bool bForceRefresh);

    void ResetElementsToManipulate(const bool bClearList = true);
#pragma endregion

#pragma region Function - Gizmo Widget
    void UpdatePivotLocationForSelection(bool bOnChange);

    void SetPivot(FVector NewPivot, bool bSnapPivotToGrid, bool bIgnoreAxis, bool bAssignPivot = false);

    void ResetPivot();

    void ApplyDeltaToSelectedElements(const FTransform& InDeltaTransform);
#pragma endregion
};
