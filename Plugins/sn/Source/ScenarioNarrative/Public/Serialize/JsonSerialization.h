// JsonSerialization.h
// @author : xiangyp
// @date : 2025/06/16 17:12:17
// @desc : json序列化

#pragma once

class SCENARIONARRATIVE_API FJsonHelper
{
public:
    static const TCHAR* IgnoreFlag;
    static const TCHAR* PropertyFlag;

    static TSharedPtr<FJsonObject> ToJsonObject(const UObject* Obj);

private:
    static void SerializeStructPropertyAsJsonObjectField(const void* InnerPropData, const FStructProperty* StructProperty, const TSharedPtr<FJsonObject>& StructObject, TSet<const UObject*>& TraversedObjects);
    static TArray<TSharedPtr<FJsonValue>> SerializePropertyAsJsonArray(const void* Data, FArrayProperty* Property, TSet<const UObject*>& TraversedObjects);
    static void SerializePropertyAsJsonObjectField(const void* Data, TSharedPtr<FJsonObject> OuterObject, FProperty* Property, TSet<const UObject*>& TraversedObjects);
};
