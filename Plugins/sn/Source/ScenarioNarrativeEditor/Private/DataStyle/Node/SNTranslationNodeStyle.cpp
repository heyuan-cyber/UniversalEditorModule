#include "SNTranslationNodeStyle.h"

FSNTranslationNodeStyle::FSNTranslationNodeStyle()
{
    BottomColor = FColor(0, 112, 224);
}

FString FSNTranslationNodeStyle::GetTypeName() const
{
    return TEXT("位移节点");
}
