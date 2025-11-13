// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNMontageSetAsset.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class SCENARIONARRATIVE_API USNAnimationClipData : public UObject
{
    GENERATED_BODY()

public:
    USNAnimationClipData() : BeginFrame(0), EndFrame(0), PlayRate(1.f)
    {
        Name = MakeUniqueObjectName(nullptr, StaticClass(), FName("New Clip")).ToString();
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int BeginFrame;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int EndFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate;

    UPROPERTY(EditInstanceOnly)
    TObjectPtr<UAnimationAsset> Asset;
};

UCLASS(BlueprintType, Blueprintable)
class SCENARIONARRATIVE_API USNMontageSetAsset : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimSequenceBase> Animation = nullptr;

    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
    TArray<TObjectPtr<USNAnimationClipData>> Clips;

    UFUNCTION()
    USNAnimationClipData* CreateNewClip();

    UFUNCTION()
    USNAnimationClipData* GetClip(const FString& InName) const;

    TArray<UAnimMontage> CreateMontages() const;
};
