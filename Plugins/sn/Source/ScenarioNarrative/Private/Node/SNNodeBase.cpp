#include "Node/SNNodeBase.h"

#include "ScenarioNarrativeDefines.h"
#include "Track/SNTrackBase.h"

float USNNodeBase::Length() const
{
    return FMath::Max(0, EndTime - BeginTime);
}

bool USNNodeBase::CanAttachTo(const UClass* TargetTrack) const
{
    return AttachedToTracks().Contains(TargetTrack)
               ? true
               : AttachedToTracks().Contains(USNTrackBase::StaticClass());
}

TSet<UClass*> USNNodeBase::AttachedToTracks() const
{
    return { USNTrackBase::StaticClass() };
}

// 使用INT32_MAX应该可以满足需求
// WTF: 不用FLT_MAX防止GetMaxLength() + Value越界...
float USNNodeBase::GetMaxLength() const
{
    return static_cast<float>(INT32_MAX);
}

float USNNodeBase::GetMinLength() const
{
    return SN_ONE_FRAME_TIME;
}

bool USNNodeBase::Validate() const
{
    return IF_WITH_EDITOR(bIsValid, true);
}

TSharedPtr<FJsonObject> USNNodeBase::NodeDataToJson()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    return JsonObject;
}
