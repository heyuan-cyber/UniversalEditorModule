#include "Task/SNPreviewTaskRegistry.h"

#include "Node/SNAnimationNode.h"
#include "Task/SNPreviewTaskBase.h"

FSNPreviewTaskRegistry::FSNPreviewTaskRegistry()
{
}

FSNPreviewTaskRegistry& FSNPreviewTaskRegistry::Get()
{
    static TSharedRef<FSNPreviewTaskRegistry> GPreviewTaskRegistry = MakeShared<FSNPreviewTaskRegistry>();
    return GPreviewTaskRegistry.Get();
}

void FSNPreviewTaskRegistry::Register(UClass* Type, FSNPreviewTaskBase* Task)
{
    RegisteredTask.Add(Type, Task);
}

FSNPreviewTaskBase* FSNPreviewTaskRegistry::GetPreviewTask(const USNAssetBase* Asset) const
{
    if (RegisteredTask.Contains(Asset->GetClass()))
    {
        return RegisteredTask[Asset->GetClass()];
    }
    return nullptr;
}

void FSNPreviewTaskRegistry::Clear()
{
    for (const auto& Pair : RegisteredTask)
    {
        delete Pair.Value;
    }

    RegisteredTask.Empty();
}

template <typename T>
TSharedPtr<FSNPreviewTaskBase> FSNPreviewTaskRegistry::GetPreviewTask() const
{
    return GetPreviewTask(T::StaticClass());
}
