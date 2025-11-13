#include "PreviewScene/SNAssetPreviewScene.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Preview/SNPreviewContext.h"
#include "Editor/UnrealEd/Public/PackageTools.h"
#include "Preview/SNPreviewManager.h"
#include "Node/SNSceneNode.h"


#define LOCTEXT_NAMESPACE "FSNAssetPreviewScene"

FSNAssetPreviewScene::FSNAssetPreviewScene(ConstructionValues CVS, TWeakPtr<FSNPreviewContext> PreviewContext)
    : FAdvancedPreviewScene(CVS)
{
    this->PreviewContext = PreviewContext;
    AWorldSettings* WorldSettings = GetWorld()->GetWorldSettings();
    WorldSettings->bEnableWorldBoundsChecks = false;
    SetFloorVisibility(true);
    RegistryToManager(USNSceneNode)
}

void FSNAssetPreviewScene::SpawnLevel(TSoftObjectPtr<UWorld> LevelToLoad)
{
    if (LevelToLoad.IsNull()) return;

    if (!LevelLoader)
    {
        LevelLoader = NewObject<ULevelLoader>(GetWorld());
        // 添加到根集，防止被垃圾回收
        LevelLoader->AddToRoot();
    }

    LevelLoader->LoadLevelToEditorViewport(LevelToLoad.GetLongPackageName(), GetWorld());

}

void FSNAssetPreviewScene::UnspawnLevel()
{
    if (LevelLoader)
    {
        LevelLoader->UnloadCurrentLevel();
    }
}

// 直接通过运行时接口StreamingLevel加载关卡，目前只能同步加载
void FSNAssetPreviewScene::LoadStreamingLevel(TSoftObjectPtr<UWorld> LevelToLoad)
{
    const FVector Location(0, 0, 0);
    const FRotator Rotation(0, 0, 0);
    bool bOutSuccess = false;

    LoadedLevelInstance = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        GetWorld(),
        LevelToLoad,
        Location,
        Rotation,
        bOutSuccess
    );

    if (bOutSuccess && LoadedLevelInstance)
    {
        if (LoadedLevelInstance && bOutSuccess)
        {
            GetWorld()->FlushLevelStreaming(EFlushLevelStreamingType::Full);
        }
    }
}

void FSNAssetPreviewScene::UnloadStreamingLevel()
{
    if (LoadedLevelInstance)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            if (!LoadedLevelInstance) return;

            LoadedLevelInstance->SetIsRequestingUnloadAndRemoval(true);
            GetWorld()->FlushLevelStreaming(EFlushLevelStreamingType::Full);
        }
    }
}
#undef LOCTEXT_NAMESPACE

ULevelLoader::ULevelLoader()
{
}

ULevelLoader::~ULevelLoader()
{
    if (OnLoadedDelegate.IsBound())
    {
        OnLoadedDelegate.Unbind();
    }
}

void ULevelLoader::LoadLevelToEditorViewport(const FString& LevelPath, UWorld* EditorWorld)
{
    // 获取当前编辑器世界
    CurrentEditorWorld = EditorWorld;

    if (!CurrentEditorWorld.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("No active editor world found"));
        return;
    }

    // 启动异步加载

    if (!OnLoadedDelegate.IsBound())
    {
        OnLoadedDelegate.BindUObject(this, &ULevelLoader::OnPackageLoaded);
    }

    FPackagePath PackageName;
    if (FPackagePath::TryFromPackageName(LevelPath, PackageName))
    {
        LoadPackageAsync(
            PackageName,
            NAME_None,
            OnLoadedDelegate,
            PKG_ContainsMap,
            INDEX_NONE,
            TAsyncLoadPriority(0)
        );
    }
}

void ULevelLoader::UnloadCurrentLevel()
{
    if (!CurrentStreamingLevel || !CurrentEditorWorld.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("No streaming level to unload"));
        return;
    }

    FString LevelPackageName = CurrentStreamingLevel->GetWorldAssetPackageName();

    // 从World移除StreamingLevel
    CurrentStreamingLevel->SetShouldBeVisible(false);
    CurrentStreamingLevel->SetIsRequestingUnloadAndRemoval(true);
    CurrentEditorWorld->FlushLevelStreaming();

    if (!LevelPackageName.IsEmpty())
    {
        UPackage* LevelPackage = FindPackage(nullptr, *LevelPackageName);
        if (LevelPackage->IsFullyLoaded())
        {
            TArray<UPackage*> PackagesToUnload;
            PackagesToUnload.Add(LevelPackage);
            UPackageTools::UnloadPackages(PackagesToUnload);
        }
    }

    // 清空StreamingLevel指针
    CurrentStreamingLevel = nullptr;

    UE_LOG(LogTemp, Log, TEXT("Unloaded level package: %s"), *LevelPackageName);
}

void ULevelLoader::OnPackageLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
    if (Result != EAsyncLoadingResult::Succeeded || !LoadedPackage)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load package: %s"), *PackageName.ToString());
        return;
    }

    // 处理加载的关卡
    ProcessLoadedLevel(LoadedPackage);
}

void ULevelLoader::ProcessLoadedLevel(UPackage* LevelPackage)
{
    if (!CurrentEditorWorld.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Editor world not available during level processing"));
        return;
    }

    // 查找Package中的世界对象
    UWorld* LoadedWorld = nullptr;
    ForEachObjectWithPackage(LevelPackage, [&LoadedWorld](UObject* Obj)
        {
            if (UWorld* World = Cast<UWorld>(Obj))
            {
                LoadedWorld = World;
                return false;
            }
            return true;
        }, false);

    if (!LoadedWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("No world found in package: %s"), *LevelPackage->GetName());
        return;
    }

    // 获取世界的持久关卡
    ULevel* LoadedLevel = LoadedWorld->PersistentLevel;
    if (!LoadedLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("No persistent level found in world: %s"), *LoadedWorld->GetName());
        return;
    }
    // 创建 StreamingDynamic
    CurrentStreamingLevel = NewObject<ULevelStreamingDynamic>(CurrentEditorWorld.Get(), NAME_None, RF_Transient);
    CurrentStreamingLevel->SetWorldAssetByPackageName(FName(*LoadedLevel->GetPathName()));
    CurrentStreamingLevel->SetShouldBeLoaded(true);
    //CurrentStreamingLevel->SetShouldBeVisible(true);
    //CurrentStreamingLevel->bShouldBlockOnLoad = false;
    //CurrentStreamingLevel->SetIsRequestingUnloadAndRemoval(false);

    CurrentEditorWorld->AddStreamingLevel(CurrentStreamingLevel.Get());

    CurrentEditorWorld->FlushLevelStreaming();

    UE_LOG(LogTemp, Log, TEXT("Successfully loaded level: %s"), *LoadedLevel->GetName());
}
