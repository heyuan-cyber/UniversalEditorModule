// HoverableBox.h
// @author : xiangyp
// @date : 2025/06/20 11:20:36
// @desc : 支持悬停回调的BoxWidget

#pragma once
#include "CoreMinimal.h"

class SCENARIONARRATIVEEDITOR_API SSNHoverableBox : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SSNHoverableBox){}
        SLATE_EVENT(FSimpleDelegate, OnHover)
        SLATE_EVENT(FSimpleDelegate, OnMouseEnter)
        SLATE_EVENT(FSimpleDelegate, OnMouseExit)
        SLATE_DEFAULT_SLOT(FArguments, Content)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SSNHoverableBox() override = default;

    virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
    bool bIsHovered = false;
    FSimpleDelegate OnHover;
    FSimpleDelegate OnMouseEnterCallback;
    FSimpleDelegate OnMouseExitCallback;
};
