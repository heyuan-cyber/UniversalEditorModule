#pragma once
#include "AdvancedPreviewScene.h"
#include "Preview/SNAssetPreviewBase.h"
#include "SNAssetPreviewScene.generated.h"

class ULevelStreamingDynamic;
struct FSNScenePreviewContext;
class USNNodeBase;
class ASNPreviewActor;
class FSNSequencerEditorToolkit;

DECLARE_DELEGATE(FOnLevelLoadedDelegate);

class SCENARIONARRATIVEEDITOR_API FSNAssetPreviewScene : public FAdvancedPreviewScene, public TSharedFromThis<FSNAssetPreviewScene>, FSNAssetPreviewBase
{
#pragma region Propertites
private:

    TObjectPtr<ULevelStreamingDynamic> LoadedLevelInstance;

    TObjectPtr<ULevelLoader> LevelLoader;

    TWeakPtr<FSNPreviewContext> PreviewContext;
#pragma endregion

#pragma region Function
public:
    FSNAssetPreviewScene(ConstructionValues CVS, TWeakPtr<FSNPreviewContext> PreviewContext);

    virtual ~FSNAssetPreviewScene() override = default;


    void SpawnLevel(TSoftObjectPtr<UWorld> LevelToLoad);

    void UnspawnLevel();

    // LevelStreaming 方案，同步加载且无法卸载内存
    void LoadStreamingLevel(TSoftObjectPtr<UWorld> LevelToLoad);

    void UnloadStreamingLevel();
#pragma endregion
};

UCLASS()
class ULevelLoader : public UObject
{
    GENERATED_BODY()

private:
    TWeakObjectPtr<UWorld> CurrentEditorWorld;

    TObjectPtr<ULevelStreamingDynamic> CurrentStreamingLevel;

    FLoadPackageAsyncDelegate OnLoadedDelegate;

public:
    ULevelLoader();

    virtual ~ULevelLoader();

    // 异步加载关卡到当前编辑器视口
    void LoadLevelToEditorViewport(const FString& LevelPath, UWorld* EditorWorld);

    // 卸载当前加载的关卡
    void UnloadCurrentLevel();

private:
    // 异步加载完成回调
    void OnPackageLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);

    // 处理加载的关卡
    void ProcessLoadedLevel(UPackage* LevelPackage);
};
