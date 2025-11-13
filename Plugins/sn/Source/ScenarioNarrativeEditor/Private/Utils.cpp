#include "Utils.h"

#include "SNAssetBase.h"
#include "ClassViewerModule.h"
#include "ComponentAssetBroker.h"
#include "CurveEditor.h"
#include "RichCurveEditorModel.h"
#include "ScenarioNarrativeDefines.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor/ClassViewer/Private/UnloadedBlueprintData.h"
#include "Serialize/JsonSerialization.h"

const float ScenarioNarrativeEditor::FUtils::FRAME_RATE = ::SN_FRAME_RATE;
const float ScenarioNarrativeEditor::FUtils::ONE_FRAME_TIME = ::SN_ONE_FRAME_TIME;

TSharedRef<SWidget> ScenarioNarrativeEditor::FUtils::MakeNewObjectPicker(UClass* ObjectClass, const FOnActionBPClassPicked& OnClassPicked, const FOnFilterUClass& FilterUClass, const FOnFilterBlueprintData& FilterBlueprintData)
{
    class FBlueprintNodeFilter : public IClassViewerFilter
    {
    public:
        FBlueprintNodeFilter(UClass* InClass, const FOnFilterUClass& InFilterUClass, const FOnFilterBlueprintData& InFilterBlueprintData)
            : ObjectClass(InClass), FilterUClass(InFilterUClass), FilterBlueprintData(InFilterBlueprintData)
        {
        }

        virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
                                    TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
        {
            if (!InClass->IsChildOf(ObjectClass))
                return false;

            if (InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Transient | CLASS_Hidden | CLASS_Deprecated))
                return false;

            if (InClass->GetName().StartsWith(TEXT("SKEL_")) || InClass->GetName().StartsWith(TEXT("REINST_")))
                return false;

            if (FilterUClass.IsBound())
                return FilterUClass.Execute(InClass);

            return true;
        }

        virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
                                            const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
                                            TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
        {
            if (!InUnloadedClassData->IsChildOf(ObjectClass))
                return false;

            if (InUnloadedClassData->HasAnyClassFlags(CLASS_Abstract | CLASS_Transient | CLASS_Hidden | CLASS_Deprecated))
                return false;

            if (InUnloadedClassData->GetClassName()->StartsWith(TEXT("SKEL_")) || InUnloadedClassData->GetClassName()->StartsWith(TEXT("REINST_")))
                return false;

            if (FilterBlueprintData.IsBound())
                return FilterBlueprintData.Execute(InUnloadedClassData);

            return true;
            // const bool bChildOfObjectClass = InUnloadedClassData->IsChildOf(ObjectClass);
            // const bool bMatchesFlags = !InUnloadedClassData->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated | CLASS_Abstract);
            //
            // return bChildOfObjectClass && bMatchesFlags;
        }

    private:
        UClass* ObjectClass;
        const FOnFilterUClass FilterUClass;
        const FOnFilterBlueprintData FilterBlueprintData;
    };

    FClassViewerInitializationOptions InitOptions;
    InitOptions.Mode = EClassViewerMode::ClassPicker;
    InitOptions.bShowObjectRootClass = false;
    InitOptions.bShowUnloadedBlueprints = true;
    InitOptions.bShowNoneOption = false;
    InitOptions.bEnableClassDynamicLoading = true;
    InitOptions.bExpandRootNodes = true;
    InitOptions.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
    InitOptions.ClassFilters.Add(MakeShared<FBlueprintNodeFilter>(ObjectClass, FilterUClass, FilterBlueprintData));
    InitOptions.bShowBackgroundBorder = false;

    FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

    return SNew(SBox).MinDesiredWidth(500.0f).MaxDesiredHeight(400.0f)
    [
        ClassViewerModule.CreateClassViewer(InitOptions, FOnClassPicked(OnClassPicked))
    ];
}

bool ScenarioNarrativeEditor::FUtils::IsNodeWidgetIntersect(const SWidget* Lhs, const SWidget* Rhs)
{
    if (!Lhs || !Rhs)
    {
        return false;
    }

    const auto& RectLeft = Lhs->GetCachedGeometry().GetLayoutBoundingRect();
    const auto& RectRight = Rhs->GetCachedGeometry().GetLayoutBoundingRect();

    return FSlateRect::DoRectanglesIntersect(RectLeft, RectRight);
}

TArray<TObjectPtr<USceneComponent>> ScenarioNarrativeEditor::FUtils::CreateAttachedObjectToSkeleton(
    USkeletalMeshComponent* SkeletalMeshComponent, const USkeleton* Skeleton)
{
    TArray<TObjectPtr<USceneComponent>> Result;
    for (int32 Idx = 0; Idx < Skeleton->PreviewAttachedAssetContainer.Num(); ++Idx)
    {
        const FPreviewAttachedObjectPair& Pair = Skeleton->PreviewAttachedAssetContainer[Idx];
        if (GetAttachedComponent(SkeletalMeshComponent, Pair))
        {
            continue;
        }

        if (USceneComponent* Comp = AddSceneComponentViaAttachedObjectPair(SkeletalMeshComponent, Pair))
        {
            Result.Add(Comp);
        }
    }

    return Result;
}

USceneComponent* ScenarioNarrativeEditor::FUtils::GetAttachedComponent(const USceneComponent* Component, const FPreviewAttachedObjectPair& Pair)
{
    if (!Component)
    {
        return nullptr;
    }

    for (USceneComponent* ChildComponent : Component->GetAttachChildren())
    {
        UObject* Asset = FComponentAssetBrokerage::GetAssetFromComponent(ChildComponent);
        if (Asset == Pair.GetAttachedObject() && ChildComponent->GetAttachSocketName() == Pair.AttachedTo)
        {
            return ChildComponent;
        }
    }

    return nullptr;
}

USceneComponent* ScenarioNarrativeEditor::FUtils::AddSceneComponentViaAttachedObjectPair(USkeletalMeshComponent* SkelMeshComponent,
                                                                                    const FPreviewAttachedObjectPair& Pair)
{
    AActor* PreviewActor = SkelMeshComponent->GetOwner();
    TSubclassOf<UActorComponent> ComponentClass = FComponentAssetBrokerage::GetPrimaryComponentForAsset(Pair.GetAttachedObject()->GetClass());
    if (*ComponentClass && ComponentClass->IsChildOf(USceneComponent::StaticClass()))
    {
        if (USceneComponent* Comp = Cast<USceneComponent>(PreviewActor->AddComponentByClass(ComponentClass, true, FTransform::Identity, false)))
        {
            FComponentAssetBrokerage::AssignAssetToComponent(Comp, Pair.GetAttachedObject());
            if (!Comp->IsAttachedTo(SkelMeshComponent))
            {
                Comp->AttachToComponent(SkelMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, Pair.AttachedTo);
            }
            return Comp;
        }
    }
    return nullptr;
}

TSharedPtr<FCurveEditor> ScenarioNarrativeEditor::FUtils::MakeCurveEditor(FRichCurve* Curve, UObject* Owner, bool bShowKey, FLinearColor LineColor)
{
    auto Editor = MakeShared<FCurveEditor>();
    Editor->InitCurveEditor({});

    if (Owner)
    {
        TUniquePtr<FRichCurveEditorModelRaw> CurveModel = MakeUnique<FRichCurveEditorModelRaw>(Curve, Owner);
        CurveModel->SetColor(LineColor, false);
        CurveModel->SetIsKeyDrawEnabled(bShowKey);

        FCurveModelID CurveID = Editor->AddCurve(MoveTemp(CurveModel));
        Editor->PinCurve(CurveID);
    }

    return Editor;
}

bool ScenarioNarrativeEditor::FUtils::IsTopOfWidget(const SWidget* Widget, const float& Y)
{
    ensure(Widget);
    const float Delta = Y - Widget->GetCachedGeometry().AbsolutePosition.Y;
    if (Delta < 0)
        return false;
    // const float LocalY = Widget->GetCachedGeometry().AbsoluteToLocal({0, Y}).Y;

    return Delta <= Widget->GetCachedGeometry().Size.Y / 2.f;
}

bool ScenarioNarrativeEditor::FUtils::ExportActionAssetToJsonFile(const USNAssetBase* Asset, const FString& SavePath)
{
    if (!Asset)
        return false;

    FString JsonStr;
    auto JsonObj = FJsonHelper::ToJsonObject(Asset);
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);
    return FFileHelper::SaveStringToFile(JsonStr, *SavePath, FFileHelper::EEncodingOptions::ForceUTF8);
}
