// PhysicsCastNode.h
// @author : xiangyp
// @date : 2025/05/12 15:49:49
// @desc : 物理投射节点

#pragma once
#include "SNNodeBase.h"
#include "ScenarioNarrativeDefines.h"
#include "SNPhysicsCastNode.generated.h"

UCLASS(Blueprintable)
class SCENARIONARRATIVE_API USNPhysicsCastNode : public USNNodeBase
{
    GENERATED_BODY()
public:
    USNPhysicsCastNode();
    virtual ~USNPhysicsCastNode() override = default;

    UPROPERTY(EditAnywhere)
    bool bIsFollowActorLocation = false;

    UPROPERTY(EditAnywhere, meta=(EditCondition = "bIsFollowActorLocation", EditConditionHides))
    FString SocketName;

    UPROPERTY(EditAnywhere, meta=(EditCondition = "!bIsFollowActorLocation && !SocketName.IsEmpty()", EditConditionHides))
    FVector Start;

    UPROPERTY(EditAnywhere, DisplayName= "Length")
    float CastLength;

    UPROPERTY(EditAnywhere)
    FVector LocationOffset;

    UPROPERTY(EditAnywhere)
    bool bIsFollowActorRotation = false;

    // UPROPERTY(EditAnywhere, meta=(EditCondition = "bIsFollowActorRotation", EditConditionHides))
    // bool bReverseDirection = false;

    UPROPERTY(EditAnywhere)
    FRotator OrientationOffset;

    UPROPERTY(EditAnywhere, meta=(EditCondition = "!bIsFollowActorRotation", EditConditionHides))
    FRotator Orientation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere)
    bool bIsSingleCast = true;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<ESNTraceShapeType::Type> Shape = ESNTraceShapeType::Line;

    UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape == ETraceShapeType::Capsule || Shape == ETraceShapeType::Sphere", EditConditionHides))
    float Radius = 1.f;
    UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape == ETraceShapeType::Capsule", EditConditionHides))
    float HalfHeight = 0.5f;
    UPROPERTY(EditAnywhere, meta=(EditCondition = "Shape == ETraceShapeType::Box", EditConditionHides))
    FVector HalfExtent = FVector::OneVector;

    DECLARE_EVENT_OneParam(UPhysicsCastNode, FOnActionCastHit, const FHitResult&);
    FOnActionCastHit OnActionCastHit;

#if WITH_EDITORONLY_DATA
    bool bIsShowColliderMesh;
#endif

    virtual TSet<UClass*> AttachedToTracks() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
