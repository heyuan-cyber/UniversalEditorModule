#include "SNRecordInputNodeStyle.h"

FSNRecordInputNodeStyle::FSNRecordInputNodeStyle()
{
    BottomColor = FColor(255, 255, 127);
}

FString FSNRecordInputNodeStyle::GetTypeName() const
{
    return TEXT("记录输入节点");
}
