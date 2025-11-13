// CustomActionDrawerBase.h
// @author : xiangyp
// @date : 2025/04/24 14:35:37
// @desc :

#pragma once

#include "CoreMinimal.h"

class FSNSequencerContext;
class USNAssetBase;

class SCENARIONARRATIVEEDITOR_API FSNCustomActionDrawerBase : public TSharedFromThis<FSNCustomActionDrawerBase>
{
public:
    virtual ~FSNCustomActionDrawerBase() = default;

    virtual bool IsDrawDefaultStyle() const { return true; }
    virtual void OnSlateRepaint(const USNAssetBase* Asset, const TSharedPtr<SOverlay>& Slot, const FSNSequencerContext* Context) = 0;
    virtual int OnWidgetPaint(const USNAssetBase* Asset, const FSNSequencerContext* Context, const FPaintArgs& Args,
                              const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                              int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const = 0;

    UObject* CreateTransientUObject(UClass* ObjectClass)
    {
        // 使用 Transient 包创建对象
        UObject* TransientObject = NewObject<UObject>(
            GetTransientPackage(), // 使用临时包
            ObjectClass,           // 对象的类
            NAME_None,             // 不指定名称（自动生成）
            RF_Transient           // 标记为临时对象
        );

        return TransientObject;
    }

    // 2. 针对自定义类的专用创建函数
    template<typename T>
    T* CreateTransientObject()
    {
        return NewObject<T>(
            GetTransientPackage(),
            T::StaticClass(),
            NAME_None,
            RF_Transient
        );
    }
};
