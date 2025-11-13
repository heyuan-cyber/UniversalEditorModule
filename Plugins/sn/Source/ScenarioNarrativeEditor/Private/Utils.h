// Utils.h
// @author : xiangyp
// @date : 2025/04/24 15:01:19
// @desc : 全局工具函数和不那么好分类的玩意

#pragma once
// #include "AssetRegistry/AssetRegistryModule.h"

class IUnloadedBlueprintData;
class FClassViewerNode;
class USNAssetBase;
struct FPreviewContext;
struct FCurveModelID;
class FCurveEditor;
class USNNodeBase;
class USNSequencer;

namespace ScenarioNarrativeEditor
{
    DECLARE_DELEGATE_OneParam(FOnActionBPClassPicked, UClass*)
    DECLARE_DELEGATE_RetVal_OneParam(bool, FOnFilterUClass, const UClass*)
    DECLARE_DELEGATE_RetVal_OneParam(bool, FOnFilterBlueprintData, const TSharedRef<const IUnloadedBlueprintData>)

    class FUtils
    {
    public:
        static const float FRAME_RATE;
        static const float ONE_FRAME_TIME;

        // 获取某个基类的所有非抽象派生类
        template <class T = UClass>
        static TArray<UClass*> GetUObjectDerivedClasses()
        {
            TArray<UClass*> Result;

            for (TObjectIterator<UClass> Iter; Iter; ++Iter)
            {
                UClass* Type = *Iter;
                if (!Type->IsChildOf(T::StaticClass()))
                    continue;

                if (Type->HasAnyClassFlags(CLASS_Abstract | CLASS_Transient | CLASS_Hidden | CLASS_Deprecated))
                    continue;

                // 过滤掉蓝图类
                if (Type->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
                    continue;

                if (Type->GetName().StartsWith(TEXT("SKEL_")) || Type->GetName().StartsWith(TEXT("REINST_")))
                    continue;

                Result.Add(Type);

            }
            return Result;
        }

        static TSharedRef<SWidget> MakeNewObjectPicker(UClass* ObjectClass, const FOnActionBPClassPicked& OnClassPicked, const FOnFilterUClass& FilterUClass, const FOnFilterBlueprintData& FilterBlueprintData);

        // 判断两个Widget是否相交
        static bool IsNodeWidgetIntersect(const SWidget* Lhs, const SWidget* Rhs);

        // 自动生成骨骼附件
        static TArray<TObjectPtr<USceneComponent>> CreateAttachedObjectToSkeleton(USkeletalMeshComponent* SkeletalMeshComponent,
                                                                                  const USkeleton* Skeleton);

        // 通过附件配置获取对应的组件
        static USceneComponent* GetAttachedComponent(const USceneComponent* Component, const FPreviewAttachedObjectPair& Pair);

        // 添加对应附件
        static USceneComponent* AddSceneComponentViaAttachedObjectPair(USkeletalMeshComponent* SkelMeshComponent,
                                                                       const FPreviewAttachedObjectPair& Pair);

        // 创建一个曲线编辑器
        static TSharedPtr<FCurveEditor> MakeCurveEditor(FRichCurve* Curve, UObject* Owner, bool bShowKey = false,
                                                        FLinearColor LineColor = FLinearColor::White);

        // 该位置是否位于某组件上半部分
        static bool IsTopOfWidget(const SWidget* Widget, const float& Y);

        // 导出某个序列资源到json文件
        static bool ExportActionAssetToJsonFile(const USNAssetBase* Asset, const FString& SavePath);
    };
}
