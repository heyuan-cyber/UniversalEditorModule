// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNCrossableNode.h"
#include "KeyFrame/SNKeyFrame.h"
#include "SNPerformerNode.generated.h"

/**
 *
 */
UCLASS()
class SCENARIONARRATIVE_API USNPerformerNode : public USNCrossableNode, public ISNKeyFrameInterface
{
    GENERATED_BODY()

public:
    /* 关键帧数据 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSNKeyFrame> KeyFrames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> PreviewSkeleton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimSequenceBase> Animation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate = 1.f;

    UPROPERTY()
    FTransform InitialTransform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bActiveLoopMode = false;

    virtual TSet<UClass*> AttachedToTracks() const override;


    /*
     * Implement Of ISNKeyFrameInterface
     */
    virtual TArray<FSNKeyFrame>& GetKeyFrames() override { return KeyFrames; };

    void SetKeyFramesTime(int32 index, float NewTime) override;
};
