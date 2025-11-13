// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNPreviewHUD.h"
#include "SNPreviewDialogHUD.generated.h"

struct FDNNodeData;

UCLASS()
class SCENARIONARRATIVEEDITOR_API USNPreviewDialogHUD : public USNPreviewHUD
{
    GENERATED_BODY()
public:

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Dialog Data")
    FString StateName;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Dialog Data")
    FString StateContent;

    UPROPERTY(BlueprintReadWrite, Category = "Dialog Data")
    FString ImagePath;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Dialog Data")
    TArray<FString> OutPinTextArr;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Dialog Data")
    bool bIsSelector;

    UFUNCTION(BlueprintNativeEvent)
    void Update(const FDNNodeData& DialogData);

};
