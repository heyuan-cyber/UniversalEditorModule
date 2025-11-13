// SNTypeRegistryHelper.h
// @author : xiangyp
// @date : 2025/05/06 14:55:17
// @desc : 统一管理各种编辑器内注册相关的东西

#pragma once

class FAssetTypeActions_Base;

class FSNTypeRegistryHelper
{
public:
    FSNTypeRegistryHelper();
    ~FSNTypeRegistryHelper();

    void Initialize();
    void UnregisterAll();

    static FSNTypeRegistryHelper& Get();
    static void RegisterStartupCallback();

    const FSlateBrush* GetBrush(const FName PropertyName) const;

private:
    bool bIsInited;
    TArray<TSharedRef<FAssetTypeActions_Base>> TypeSNs;
    TSharedPtr<FSlateStyleSet> StyleSet;

    void RegisterSlateStyleSet();
    void RegisterDataStyle();

    void RegisterDrawer();
    void RegisterAssetTypeActions();
    void RegisterNodeTasks();
    void RegisterNodePreviewers();

    FString InContent(const FString& RelativePath, const ANSICHAR* Extension);

    static void CreateMontageSetViaAnimationSequence();
};
