// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "SNTrackBase.h"
#include "Misc/FileHelper.h"
#include "Blueprint/UserWidget.h"
#include "SNDialogTrack.generated.h"

struct FDNNodeData;

UCLASS()
class SCENARIONARRATIVE_API USNDialogTrack : public USNTrackBase
{
    GENERATED_BODY()

public:
#pragma region Properties

    FFilePath DialogFilePath;

    UPROPERTY(VisibleAnywhere, Category = "Dialog Config", meta = (ResetToDefault))
    FString DialogName = "";

    UPROPERTY(VisibleAnywhere, Category = "Dialog Config", meta = (ResetToDefault))
    TArray<FDNNodeData> DialogData = {};

    UPROPERTY(EditAnywhere, Category = "Dialog Config", meta = (ResetToDefault))
    TSubclassOf<UUserWidget> HUDClass;

#pragma endregion




#pragma region Function

    UFUNCTION(CallInEditor, Category = "Dialog Config")
    void SelectCSVFile();

    void LoadDialogData();

#pragma endregion


};
