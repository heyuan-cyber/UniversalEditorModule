#pragma once

#include "SNCrossableNode.h"
#include "SNCameraNode.generated.h"

USTRUCT(BlueprintType)
struct FSNCameraKeyFrame
{
    GENERATED_BODY()

    // World Time In Timeline
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float AbsoluteTime = -1;

    // Relative Time In Node
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Time = -1;  // 不初始化的话IsValid()会有问题

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Position;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float FOV;

    bool IsValid() const
    {
        return Time >= 0.0f;
    }
};

UCLASS()
class SCENARIONARRATIVE_API USNCameraKeyFrameWrapper : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FSNCameraKeyFrame CameraKeyFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 KeyFrameId;

    UPROPERTY()
    TWeakObjectPtr<USNCameraNode> SourceNode;
};

UENUM(BlueprintType)
enum class ESNCameraFocusMode : uint8
{
    None        UMETA(DisplayName = "无聚焦"),
    Actor       UMETA(DisplayName = "聚焦到对象"),
    Location    UMETA(DisplayName = "聚焦到位置")
};

UCLASS()
class SCENARIONARRATIVE_API USNCameraNode : public USNCrossableNode
{
    GENERATED_BODY()

public:
    USNCameraNode();
    virtual ~USNCameraNode() override = default;

    virtual TSet<UClass*> AttachedToTracks() const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
    ESNCameraFocusMode FocusMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode == ESNCameraFocusMode::Actor", EditConditionHides))
    AActor* FocusTargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode == ESNCameraFocusMode::Location", EditConditionHides))
    FVector FocusTargetPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode != ESNCameraFocusMode::None", EditConditionHides))
    bool bBlendFocusRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode != ESNCameraFocusMode::None && bBlendFocusRotation", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
    float FocusBlendAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    mutable TArray<FSNCameraKeyFrame> KeyFrames;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    void SetKeyFramesTime(int32 index, float NewTime);

    void SortKetFramesByTime();

    TArray<FSNCameraKeyFrame> GetKetFramesByCurrentTime(float CurrentTime);

    virtual TSharedPtr<FJsonObject> NodeDataToJson() override;
};
