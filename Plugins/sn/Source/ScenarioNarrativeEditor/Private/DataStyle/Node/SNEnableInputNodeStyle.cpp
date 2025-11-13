#include "SNEnableInputNodeStyle.h"

FSNEnableInputNodeStyle::FSNEnableInputNodeStyle()
{
    // BackgroundColor = FColor(255, 255, 127);
    BottomColor = FColor(127, 255, 255);
}

FString FSNEnableInputNodeStyle::GetTypeName() const
{
    return TEXT("输入控制节点");
}
