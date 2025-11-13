#include "Sequencer/SNSequencer.h"

#include "Serialize/JsonSerialization.h"
#include "Track/SNTrackBase.h"

#define LOCTEXT_NAMESPACE "SNSequencer"

USNSequencer::USNSequencer()
{
}

TObjectPtr<USNTrackBase> USNSequencer::MakeNewTrack(const UClass* Cls, const FName& InDisplayName)
{
    TObjectPtr<USNTrackBase> Track = NewObject<USNTrackBase>(this, Cls, NAME_None, RF_Transactional);
    Track->DisplayName = InDisplayName.IsNone() ? MakeUniqueObjectName(this, Cls, FName(Cls->GetName())) : InDisplayName;
    Tracks.Add(Track);
#if WITH_EDITOR
    Modify();
#endif
    return Track;
}

float USNSequencer::GetMaxLength() const
{
    float Length = 0;
    for (const auto& Track : Tracks)
    {
        if (!Track)
            continue;

        Length = FMath::Max(Length, Track->GetLength());
    }
    return Length;
}

TSharedRef<FJsonObject> USNSequencer::ToJsonObject() const
{
    TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    TArray<TSharedPtr<FJsonValue>> TrackObjects;
    for (const auto& Track : Tracks)
    {
        if (!Track)
            continue;

        TrackObjects.Add(MakeShareable(new FJsonValueObject(Track->ToJsonObject())));
    }

    if (FProperty* TracksProp = FindFProperty<FProperty>(this->GetClass(), TEXT("Tracks")))
    {
        if (TracksProp->HasMetaData(FJsonHelper::PropertyFlag))
        {
            FString FieldName = TracksProp->GetMetaDataText(TEXT("Tracks")).ToString();
            if (FieldName.IsEmpty())
                FieldName = TracksProp->GetAuthoredName();

            JsonObject->SetArrayField(FieldName, TrackObjects);
        }
        else
        {
            JsonObject->SetArrayField(TracksProp->GetAuthoredName(), TrackObjects);
        }
    }
    else
    {
        JsonObject->SetArrayField(TEXT("Tracks"), TrackObjects);
    }

    return JsonObject;
}

template <typename T>
TObjectPtr<T> USNSequencer::MakeNewTrack(const FName& InDisplayName)
{
    return MakeNewTrack(T::StaticClass(), InDisplayName);
}

#undef LOCTEXT_NAMESPACE
