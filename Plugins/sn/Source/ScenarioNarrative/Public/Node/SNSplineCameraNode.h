#pragma once

#include "SNCrossableNode.h"
#include "SNSplineCameraNode.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct FSNSplineCameraKeyFrame
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Time = -1;  // 不初始化的话IsValid()会有问题

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 PointInputKey;

    bool IsValid() const
    {
        return Time >= 0.0f;
    }
};

UCLASS()
class SCENARIONARRATIVE_API USNSplineCameraKeyFrameWrapper : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    FSNSplineCameraKeyFrame SplineCameraKeyFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 KeyFrameId;

    UPROPERTY()
    TWeakObjectPtr<USNSplineCameraNode> SourceNode;
};

UENUM(BlueprintType)
enum class ESNSplineCameraFocusMode : uint8
{
    None        UMETA(DisplayName = "无聚焦"),
    Actor       UMETA(DisplayName = "聚焦到对象"),
    Location    UMETA(DisplayName = "聚焦到位置")
};

UCLASS()
class SCENARIONARRATIVE_API USNSplineCameraNode : public USNCrossableNode
{
    GENERATED_BODY()

public:
    USNSplineCameraNode();
    virtual ~USNSplineCameraNode() override = default;

    virtual TSet<UClass*> AttachedToTracks() const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
    ESNSplineCameraFocusMode FocusMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode == ESNSplineCameraFocusMode::Actor", EditConditionHides))
    AActor* FocusTargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode == ESNSplineCameraFocusMode::Location", EditConditionHides))
    FVector FocusTargetPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode != ESNSplineCameraFocusMode::None", EditConditionHides))
    bool bBlendFocusRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "FocusMode != ESNSplineCameraFocusMode::None && bBlendFocusRotation", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
    float FocusBlendAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    mutable TArray<FSNSplineCameraKeyFrame> KeyFrames;

    // 添加样条线组件引用
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Camera")
    TWeakObjectPtr<USplineComponent> SplineComponent;

    float GetSplineDistanceForKeyFrame(int32 KeyFrameIndex) const;
    float GetInterpolatedSplineDistance(float CurrentTime, int32& OutPrevKeyFrame, int32& OutNextKeyFrame,
                                        float& OutAlpha) const;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    void SetKeyFramesTime(int32 index, float NewTime);
    bool GetCameraTransformAtTime(float CurrentTime, FVector& OutLocation, FRotator& OutRotation) const;
    void SortKeyFramesByTime();

    TArray<FSNSplineCameraKeyFrame> GetKeyFramesByCurrentTime(float CurrentTime);
    virtual TSharedPtr<FJsonObject> NodeDataToJson() override;
};
