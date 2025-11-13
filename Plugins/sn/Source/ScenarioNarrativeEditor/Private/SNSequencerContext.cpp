// SNSequencerContext.cpp
// @author : xiangyp
// @date : 2025/04/22 15:43:25
// @desc :
#include "SNSequencerContext.h"
#include "Node/SNNodeBase.h"
#include "PlaybackController/SequencerPlaybackController.h"
#include "Preview/SNAssetPreviewCamera.h"
#include "Preview/SNPreviewManager.h"
#include "Preview/UI/SNAssetPreviewHUD.h"
#include "PreviewScene/SNAssetPreviewScene.h"
#include "Sequencer/SNSequencer.h"
#include "SequencerEditor/SNTrackArea.h"
#include "SequencerEditor/SNTrackHierarchy.h"
#include "SequencerEditor/SNTrackWidget.h"
#include "Track/SNTrackBase.h"

void FSNSequencerContext::SetSelectNode(const TObjectPtr<USNNodeBase>& Node)
{
    OnSelectChanged.Broadcast(Node);
    if (CurrentSelectedNode == Node)
    {
        return;
    }

    CurrentSelectedUObject = nullptr;
    CurrentSelectedNode = Node;
    OnNodeSelectChanged.Broadcast();
}

void FSNSequencerContext::SetSelectTrack(const TObjectPtr<USNTrackBase>& Track)
{
    OnSelectChanged.Broadcast(Track);
    if (CurrentSelectedTrack == Track)
    {
        return;
    }

    CurrentSelectedUObject = nullptr;
    CurrentSelectedTrack = Track;
    OnTrackSelectChanged.Broadcast();
}

void FSNSequencerContext::SetSelectUObject(const TObjectPtr<UObject>& UObject)
{
    CurrentSelectedUObject = UObject;
    OnSelectUObjectChanged.Broadcast(UObject);
}

void FSNSequencerContext::MoveTrack(USNTrackBase* Track, USNTrackBase* Target, bool bIsAfter)
{
    ensure(Asset && Asset->Tracks.Contains(Track) && Asset->Tracks.Contains(Target));

    int32 RawIndex = Asset->Tracks.IndexOfByKey(Track);
    Asset->Tracks.Remove(Track);

    int32 Index = Asset->Tracks.IndexOfByKey(Target);
    int32 TargetIndex = FMath::Clamp(bIsAfter ? Index + 1 : Index, 0, Asset->Tracks.Num());

    if (Asset->Tracks.IsValidIndex(TargetIndex))
        Asset->Tracks.Insert(Track, TargetIndex);
    else
        Asset->Tracks.Add(Track);

    if (Asset->Tracks.IndexOfByKey(Track) != RawIndex)
    {


        TrackHierarchyWidget->Repaint();
        TrackAreaWidget->Repaint();
        SetSelectTrack(Track);
        Asset->Modify();
    }
}

bool FSNSequencerContext::IsIntersectionWithOther(const TObjectPtr<USNNodeBase>& Node) const
{
    if (!Node)
    {
        return false;
    }

    for (const auto& Pair : NodeToWidget)
    {
        if (Pair.Key == Node)
        {
            continue;
        }

        if (Pair.Key->Parent != Node->Parent)
        {
            continue;
        }

        if (!(Pair.Key->BeginTime >= Node->EndTime || Node->BeginTime >= Pair.Key->EndTime))
        {
            return true;
        }
    }

    return false;
}

void FSNSequencerContext::SetTrackIsDragging(const SSNTrackWidget* Target)
{
    for (const auto& Pair : TrackToWidget)
    {
        if (Pair.Value == Target)
        {
            Pair.Value->SetIsDrop(false);
            continue;
        }

        Pair.Value->SetIsDrop(Pair.Key == CurrentHoveredTrack);
    }
}

void FSNSequencerContext::ClearTrackDragging()
{
    for (const auto& Pair : TrackToWidget)
    {
        Pair.Value->SetIsDrop(false);
    }
}

bool FSNSequencerContext::SetTimeIfNearlyEqualOther(const TObjectPtr<USNNodeBase>& Node, const float& Time, float* Value,
                                                        const float Tolerance) const
{
    if (!Node)
    {
        return false;
    }

    for (const auto& Pair : NodeToWidget)
    {
        if (Pair.Key == Node)
        {
            continue;
        }

        if (FMath::IsNearlyEqual(Time, Pair.Key->BeginTime, Tolerance))
        {
            *Value = Pair.Key->BeginTime;
            return true;
        }
        if (FMath::IsNearlyEqual(Time, Pair.Key->EndTime, Tolerance))
        {
            *Value = Pair.Key->EndTime;
            return true;
        }
    }

    return false;
}

UObject* FSNSequencerContext::GetCurrentSelectedUObject()
{
    return CurrentSelectedUObject;
}

void FSNSequencerContext::SavePreviewSceneUObject()
{
    OnSaveCurrentUObjectChanged.Broadcast();
    return;
}

void FSNSequencerContext::InitializePlaybackController()
{
    if (!PlaybackController.IsValid())
    {
        PlaybackController = MakeShared<FSequencerPlaybackController>(this);

        // 设置帧率
        PlaybackController->SetFrameRate(30.0f); // 或从项目设置获取
    }
    return;
}

float FSNSequencerContext::GetMaxTrackTime()
{
    float MaxTime = 0.0f;
    for (const auto& Pair : TrackToWidget)
    {
        if(Pair.Key->GetLength() > MaxTime)
        {
            MaxTime = Pair.Key->GetLength();
        }
    }
    return MaxTime;
}

void FSNSequencerContext::TickPlaybackController(float DeltaTime)
{
    if (PlaybackController.IsValid())
    {
        PlaybackController->Tick(DeltaTime);
        // PreviewManager->Tick();
    }
}

void FSNSequencerContext::ChangeEditorCurrentTime(float Current)
{
    PreviewManager->CurrentTime = FMath::Max(0, Current);
}

void FSNSequencerContext::ExportSequenceToJson()
{
    // 创建JSON对象
    TSharedPtr<FJsonObject> JsonObject = TimelineDataToJson();
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create JSON object"));
        return ;
    }

    // 将JSON对象转换为字符串
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to serialize JSON"));
        return ;
    }

    //构造表格文件夹路径
    FString ParentDir = FPaths::GetPath(FPaths::GetPath(FPaths::ProjectDir()));
    FString TableDir = FPaths::Combine(
        ParentDir,
        TEXT("common"),
        TEXT("etc"),
        TEXT("scenario_narrative")
    );

    // 确保目录存在
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*TableDir))
    {
        PlatformFile.CreateDirectoryTree(*TableDir);
    }

    FString SavePath = TableDir / "test" + TEXT(".json");
    // 保存到文件
    if (!FFileHelper::SaveStringToFile(OutputString, *SavePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save JSON to file: %s"), *SavePath);
        return ;
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully saved timeline JSON to: %s"), *SavePath);
    return ;

    //
    // FString CleanName = ToolkitCommands.GetName();
    // if (CleanName.StartsWith("Default__"))
    // {
    //     CleanName = CleanName.Mid(9); // 移除"Default__"前缀
    //     int32 suffixPos = CleanName.Find("_C");
    //     if (suffixPos != INDEX_NONE)
    //     {
    //         CleanName = CleanName.Left(suffixPos); // 移除"_C"后缀
    //     }
    // }
    //
    // // 保存到文件
    // FString SavePath = TableDir / CleanName + TEXT(".jason");
    // FFileHelper::SaveStringToFile(CsvContent, *SavePath, FFileHelper::EEncodingOptions::ForceUTF8);
}

TSharedPtr<FJsonObject> FSNSequencerContext::TimelineDataToJson()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetNumberField("TimelineLength", GetMaxTrackTime());

    TArray<TSharedPtr<FJsonValue>> TracksArray;
    for (const auto& Pair : TrackToWidget)
    {
        TSharedPtr<FJsonValue> TrackValue = MakeShareable(new FJsonValueObject(Pair.Key->TrackDataToJson()));
        TracksArray.Add(TrackValue);
    }

    JsonObject->SetArrayField("Tracks", TracksArray);

    return JsonObject;
}
