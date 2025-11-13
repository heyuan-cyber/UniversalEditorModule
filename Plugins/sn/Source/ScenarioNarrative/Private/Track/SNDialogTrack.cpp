// Fill out your copyright notice in the Description page of Project Settings.


#include "Track/SNDialogTrack.h"
#include "Node/SNDialogNode.h"
#include "DesktopPlatformModule.h"
#include "DialogNarrative/public/DNFunctionLibrary.h"

void USNDialogTrack::SelectCSVFile()
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

    if (DesktopPlatform)
    {
        TArray<FString> OutFiles;
        const FString DefaultDir = FPaths::ProjectDir() / TEXT("common/etc/dialog");
        const FString Title = TEXT("选择CSV文件");
        const FString FileTypes = TEXT("CSV文件 (*.csv)|*.csv");

        // 打开文件对话框
        DesktopPlatform->OpenFileDialog(
            FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
            Title,
            DefaultDir,
            TEXT(""),
            FileTypes,
            EFileDialogFlags::None,
            OutFiles
        );

        if (OutFiles.Num() > 0)
        {
            DialogName = FPaths::GetCleanFilename(OutFiles[0]);
            DialogFilePath.FilePath = FPaths::ConvertRelativePathToFull(OutFiles[0]);

            LoadDialogData();

            for (auto& Node : Nodes)
            {
                auto DialogNode = Cast<USNDialogNode>(Node);
                if (DialogNode)
                {
                    DialogNode->NodeIndex = -1;
                    DialogNode->NodeData = FDNNodeData();
                }
            }
        }
    }
}

void USNDialogTrack::LoadDialogData()
{
    if (DialogFilePath.FilePath == "") return;
    UDNFunctionLibrary::LoadCSVToNodeData(DialogFilePath.FilePath, DialogData);
}
