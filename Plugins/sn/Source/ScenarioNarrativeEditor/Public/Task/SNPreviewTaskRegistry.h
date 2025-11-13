#pragma once

class FSNPreviewTaskBase;
class USNAssetBase;

class SCENARIONARRATIVEEDITOR_API FSNPreviewTaskRegistry : public TSharedFromThis<FSNPreviewTaskRegistry>
{
public:
    FSNPreviewTaskRegistry();
    virtual ~FSNPreviewTaskRegistry() = default;

    static FSNPreviewTaskRegistry& Get();

    void Register(UClass* Type, FSNPreviewTaskBase* Task);

    template <typename T = USNAssetBase>
    void Register(FSNPreviewTaskBase* Task)
    {
        Register(T::StaticClass(), Task);
    }

    FSNPreviewTaskBase* GetPreviewTask(const USNAssetBase* Asset) const;
    template <typename T = USNAssetBase>
    TSharedPtr<FSNPreviewTaskBase> GetPreviewTask() const;

    void Clear();

public:
    TMap<UClass*, FSNPreviewTaskBase*> RegisteredTask;
};
