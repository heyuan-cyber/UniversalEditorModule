#pragma once
#include "SNKeyFrame.generated.h"

class USNNodeBase;

USTRUCT(BlueprintType)
struct FSNKeyFrame
{
    GENERATED_BODY()

    // World Time In Timeline
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float AbsoluteTime = -1;

    // Relative Time In Node
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LocalTime = -1;  // 不初始化的话IsValid()会有问题

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTransform Transform;

    bool IsValid() const
    {
        return LocalTime >= 0.0f;
    }
};

UCLASS()
class SCENARIONARRATIVE_API USNKeyFrameWrapper : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSNKeyFrame KeyFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 KeyFrameId;

    UPROPERTY()
    TWeakObjectPtr<USNNodeBase> SourceNode;
};

UINTERFACE(MinimalAPI)
class USNKeyFrameInterface : public UInterface
{
    GENERATED_BODY()
};

class SCENARIONARRATIVE_API ISNKeyFrameInterface
{
    GENERATED_BODY()
public:
    virtual TArray<FSNKeyFrame>& GetKeyFrames() = 0;

    virtual void AddKeyFrame(const FSNKeyFrame& KF) { GetKeyFrames().Add(KF); }

    virtual void RemoveKeyFrame(int32 Index) { GetKeyFrames().RemoveAt(Index); }

    virtual void SortKeyFrames() { GetKeyFrames().Sort([](const FSNKeyFrame& A, const FSNKeyFrame& B) { return A.LocalTime < B.LocalTime; }); }

    virtual void SetKeyFramesTime(int32 index, float NewTime) = 0;

    bool HasKeyFrame() { return GetKeyFrames().Num() > 0; }

    void GetKeyIndexRangeOfTime(float InTime, int32& PreKeyIndex, int32& NextKeyIndex);

    FTransform CaclTargetAdditionalTransform(FTransform InitalTransform, float CurrentTime);

    FTransform GetTimeInterpTransform(FTransform InitalTransform, float LocalTime);
};
