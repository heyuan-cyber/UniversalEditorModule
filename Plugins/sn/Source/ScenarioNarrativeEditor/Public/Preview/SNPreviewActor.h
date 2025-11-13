// PreviewActor.h
// @author : xiangyp
// @date : 2025/04/28 11:09:29
// @desc : 在预览场景中的预览角色

// @update : niucy
// @date : 2025/8/15 14:44:00
// @desc : 添加编辑器视口普通Actor

#pragma once
#include "EditorUtilityActor.h"
#include "SNPreviewActor.generated.h"
class USceneComponent;

UCLASS()
class SCENARIONARRATIVEEDITOR_API ASNPreviewActor : public AActor
{
    GENERATED_BODY()

public:
    ASNPreviewActor();
    virtual ~ASNPreviewActor();

    UPROPERTY(Category = Actor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USceneComponent> SceneComponent;

    // 移动后处理
    TFunction<void(TObjectPtr<ASNPreviewActor>)> OnPostEditMove;

    virtual void PostEditMove(bool bFinished) override;
};

UCLASS()
class SCENARIONARRATIVEEDITOR_API ASNPreviewOnlyActor : public AEditorUtilityActor
{
    GENERATED_BODY()

public:
    ASNPreviewOnlyActor();

    virtual ~ASNPreviewOnlyActor() override = default;

    virtual void Reset() override;
};
