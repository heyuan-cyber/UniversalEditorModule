#include "SNHoverableBox.h"

void SSNHoverableBox::Construct(const FArguments& InArgs)
{
    bIsHovered = false;
    OnHover = InArgs._OnHover;
    OnMouseEnterCallback = InArgs._OnMouseEnter;
    OnMouseExitCallback = InArgs._OnMouseExit;
}

void SSNHoverableBox::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
    bIsHovered = true;
    OnMouseEnterCallback.ExecuteIfBound();
}

void SSNHoverableBox::OnMouseLeave(const FPointerEvent& MouseEvent)
{
    SCompoundWidget::OnMouseLeave(MouseEvent);
    bIsHovered = false;
    OnMouseExitCallback.ExecuteIfBound();
}

void SSNHoverableBox::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    if (bIsHovered && OnHover.IsBound())
        OnHover.Execute();
}
