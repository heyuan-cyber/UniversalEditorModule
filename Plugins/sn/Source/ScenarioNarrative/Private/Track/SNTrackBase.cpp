#include "Track/SNTrackBase.h"

#include "Node/SNNodeBase.h"

template <typename T>
TObjectPtr<T> USNTrackBase::MakeNewNode(const FName& InDisplayName)
{
    return MakeNewNode(T::StaticClass(), InDisplayName);
}

TObjectPtr<USNNodeBase> USNTrackBase::MakeNewNode(const UClass* Class, const FName& InDisplayName)
{
    TObjectPtr<USNNodeBase> Node = NewObject<USNNodeBase>(GetOuter(), Class, NAME_None, RF_Transactional);
    Node->DisplayName = InDisplayName.IsNone() ? MakeUniqueObjectName(GetOuter(), Class, FName(TEXT("New Node"))) : InDisplayName;
    Node->Parent = this;
    Nodes.Add(Node);

#if WITH_EDITOR
    Modify();
#endif
    return Node;
}

bool USNTrackBase::AddNode(const TObjectPtr<USNNodeBase>& Node)
{
    if (Node->GetOuter() != GetOuter())
    {
        return false;
    }

    if (Node->Parent && Node->Parent != this)
    {
        Node->Parent->RemoveNode(Node);
    }

    Nodes.AddUnique(Node);
    Node->Parent = this;
    return true;
}

bool USNTrackBase::RemoveNode(const TObjectPtr<USNNodeBase>& Node)
{
    if (Nodes.Remove(Node))
    {
        Node->Parent = nullptr;
        return true;
    }
    return false;
}

float USNTrackBase::GetLength() const
{
    float Length = 0;
    for (const auto& Node : Nodes)
    {
        if (!Node)
            continue;

        Length = FMath::Max(Length, Node->EndTime);
    }
    return Length;
}

TSharedPtr<FJsonObject> USNTrackBase::TrackDataToJson() const
{
    TSharedRef<FJsonObject> JsonObject = Super::ToJsonObject();
    JsonObject->SetStringField(TEXT("AssetType"), GetClass()->GetName());

    // Nodes字段序列化为一个字典而非数组
    TSharedRef<FJsonObject> NodesObject = MakeShared<FJsonObject>();
    for (const auto& Node : Nodes)
    {
        if (!Node)
            continue;

        NodesObject->SetObjectField(Node->GetClass()->GetName(), Node->ToJsonObject());
    }
    JsonObject->SetObjectField(TEXT("Nodes"), NodesObject);

    return JsonObject;
}
