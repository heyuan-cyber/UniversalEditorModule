#include "Track/SNCameraTrack.h"

#include "Node/SNNodeBase.h"

TSharedPtr<FJsonObject> USNCameraTrack::TrackDataToJson() const
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetStringField("StartTime", this->GetName());

    // 添加节点数组
    TArray<TSharedPtr<FJsonValue>> NodesArray;
    for (USNNodeBase* Node : Nodes)
    {
        TSharedPtr<FJsonValue> NodeValue = MakeShareable(new FJsonValueObject(Node->NodeDataToJson()));
        NodesArray.Add(NodeValue);
    }
    JsonObject->SetArrayField("Nodes", NodesArray);

    return JsonObject;
}
