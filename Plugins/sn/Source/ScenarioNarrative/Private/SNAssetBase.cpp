#include "SNAssetBase.h"

#include "Serialize/JsonSerialization.h"

USNAssetBase::USNAssetBase()
    : AssetID(MakeUniqueObjectName(GetOuter(), GetClass(), GetClass()->GetFName()).ToString())
{
}

TSharedRef<FJsonObject> USNAssetBase::ToJsonObject() const
{
    return FJsonHelper::ToJsonObject(this).ToSharedRef();
}
