#include "SNEditorDebugTrackStyle.h"

FSNEditorDebugTrackStyle::FSNEditorDebugTrackStyle()
{
    TrackIcon = FAppStyle::GetBrush("Icons.Error.Large");
    TrackEdColor = FColor(239, 53, 53);
}

FString FSNEditorDebugTrackStyle::GetTypeName() const
{
    return TEXT("编辑器预览轨道");
}
