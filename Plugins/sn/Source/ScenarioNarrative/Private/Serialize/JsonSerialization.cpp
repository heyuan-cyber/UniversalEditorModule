// Reference source: https://github.com/nicholas477/JsonSerialization.git

#include "Serialize/JsonSerialization.h"

#include "JsonObjectConverter.h"
#include "UObject/UnrealType.h"
#include "InstancedStruct.h"
#include "StructUtils/InstancedStruct.h"

#define LOCTEXT_NAMESPACE "FJsonSerialization"

const TCHAR* FJsonHelper::IgnoreFlag = TEXT("JsonIgnore");
const TCHAR* FJsonHelper::PropertyFlag = TEXT("JsonProperty");

void FJsonHelper::SerializeStructPropertyAsJsonObjectField(const void* InnerPropData, const FStructProperty* StructProperty, const TSharedPtr<FJsonObject>& StructObject, TSet<const UObject*>& TraversedObjects)
{
    const uint8* StructPropData = StructProperty->ContainerPtrToValuePtr<uint8>(InnerPropData);
    if (StructProperty->Struct == TBaseStructure<FInstancedStruct>::Get())
    {
        const FInstancedStruct& InstancedStruct = *(FInstancedStruct*)StructPropData;
        for (TFieldIterator<FProperty> PropertyItr(InstancedStruct.GetScriptStruct()); PropertyItr; ++PropertyItr)
        {
            SerializePropertyAsJsonObjectField(InstancedStruct.GetMemory(), StructObject, *PropertyItr, TraversedObjects);
        }
    }
    else
    {
        for (TFieldIterator<FProperty> PropertyItr(StructProperty->Struct); PropertyItr; ++PropertyItr)
        {
            SerializePropertyAsJsonObjectField((void*)StructPropData, StructObject, *PropertyItr, TraversedObjects);
        }
    }
}

TArray<TSharedPtr<FJsonValue>> FJsonHelper::SerializePropertyAsJsonArray(const void* Data, FArrayProperty* Property, TSet<const UObject*>& TraversedObjects)
{
    const uint8* PropData = Property->ContainerPtrToValuePtr<uint8>(Data);
    FScriptArrayHelper Helper(Property, PropData);
    TArray<TSharedPtr<FJsonValue>> ValueArray;

    for (int32 i = 0, n = Helper.Num(); i < n; ++i)
    {
        const uint8* InnerPropData = Helper.GetRawPtr(i);
        if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property->Inner)) // Array
        {
            TArray<TSharedPtr<FJsonValue>> InnerArray = SerializePropertyAsJsonArray(InnerPropData, ArrayProperty, TraversedObjects);
            ValueArray.Emplace(new FJsonValueArray(InnerArray));
        }
        else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property->Inner)) // Struct
        {
            TSharedPtr<FJsonObject> StructObject = MakeShareable(new FJsonObject);
            SerializeStructPropertyAsJsonObjectField(InnerPropData, StructProperty, StructObject, TraversedObjects);
            ValueArray.Emplace(new FJsonValueObject(StructObject));
        }
        else if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property->Inner)) // Object
        {
            const UObject* SubObject = ObjectProperty->GetObjectPropertyValue_InContainer(InnerPropData);
            if (SubObject->IsValidLowLevel() && !TraversedObjects.Contains(SubObject))
            {
                TraversedObjects.Add(SubObject);
                TSharedPtr<FJsonObject> JsonSubObject = MakeShared<FJsonObject>();
                for (TFieldIterator<FProperty> PropertyItr(SubObject->GetClass()); PropertyItr; ++PropertyItr)
                {
                    SerializePropertyAsJsonObjectField(SubObject, JsonSubObject, *PropertyItr, TraversedObjects);
                }
                ValueArray.Emplace(new FJsonValueObject(JsonSubObject));
            }
        }
        else
        {
            TSharedPtr<FJsonValue> JsonValue;
            const uint8* InnerInnerPropData = Property->Inner->ContainerPtrToValuePtr<uint8>(InnerPropData);
            ValueArray.Emplace(FJsonObjectConverter::UPropertyToJsonValue(Property->Inner, InnerInnerPropData));
        }
    }
    return ValueArray;
}

void FJsonHelper::SerializePropertyAsJsonObjectField(const void* Data, TSharedPtr<FJsonObject> OuterObject, FProperty* Property, TSet<const UObject*>& TraversedObjects)
{
    if (Property->GetName() == "UberGraphFrame"
        || Property->HasAnyPropertyFlags(CPF_Transient))
    {
        // Don't include "UberGraphFrame" or any transient properties
        return;
    }

    if (Property->HasMetaData(IgnoreFlag))
        return;

    FString FieldName = Property->HasMetaData(PropertyFlag) ?
                        Property->GetMetaDataText(PropertyFlag).ToString() :
                        Property->GetAuthoredName();

    FieldName = FieldName.IsEmpty() ? Property->GetAuthoredName() : FieldName;

    if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property)) // Array
    {
        TArray<TSharedPtr<FJsonValue>> Values = SerializePropertyAsJsonArray(Data, ArrayProperty, TraversedObjects);
        OuterObject->SetArrayField(FieldName, Values);
    }
    else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property)) // Struct
    {
        TSharedPtr<FJsonObject> StructObject = MakeShareable(new FJsonObject);
        SerializeStructPropertyAsJsonObjectField(Data, StructProperty, StructObject, TraversedObjects);
        OuterObject->SetObjectField(FieldName, StructObject);
    }
    else if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property)) // Object
    {
        // const UObject* SubObject = ObjectProperty->GetObjectPropertyValue_InContainer(Data);
        // if (SubObject->IsValidLowLevel() && !TraversedObjects.Contains(SubObject))
        // {
        //     TraversedObjects.Add(SubObject);
        //     TSharedPtr<FJsonObject> JsonSubObject = MakeShared<FJsonObject>();
        //     for (TFieldIterator<FProperty> PropertyItr(SubObject->GetClass()); PropertyItr; ++PropertyItr)
        //     {
        //         SerializePropertyAsJsonObjectField(SubObject, JsonSubObject, *PropertyItr, TraversedObjects);
        //     }
        //     OuterObject->SetObjectField(FieldName, JsonSubObject);
        // }

        // 对嵌套的UObject而言不是很好确定这东西是资源引用还是内存里的结构
        // 暂时只深入一层当作资源引用处理
        TSharedPtr<FJsonValue> JsonValue;
        const uint8* PropData = Property->ContainerPtrToValuePtr<uint8>(Data);
        OuterObject->SetField(FieldName, FJsonObjectConverter::UPropertyToJsonValue(Property, PropData));
    }
    else
    {
        TSharedPtr<FJsonValue> JsonValue;
        const uint8* PropData = Property->ContainerPtrToValuePtr<uint8>(Data);
        OuterObject->SetField(FieldName, FJsonObjectConverter::UPropertyToJsonValue(Property, PropData));
    }
}

TSharedPtr<FJsonObject> FJsonHelper::ToJsonObject(const UObject* Object)
{
    TSet<const UObject*> TraversedObjects;
    TraversedObjects.Add(Object);

    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    for (TFieldIterator<FProperty> PropertyItr(Object->GetClass()); PropertyItr; ++PropertyItr)
    {
        SerializePropertyAsJsonObjectField(Object, JsonObject, *PropertyItr, TraversedObjects);
    }

    return JsonObject;
}

#undef LOCTEXT_NAMESPACE
