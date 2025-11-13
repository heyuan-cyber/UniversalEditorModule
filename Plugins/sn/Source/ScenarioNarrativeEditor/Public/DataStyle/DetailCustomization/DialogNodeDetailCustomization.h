// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class USNDialogTrack;

class FDialogNodeDetailCustomization : public IDetailCustomization
{
public:

    static TSharedRef<FDialogNodeDetailCustomization> MakeInstance();

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:

    TArray< TWeakObjectPtr<UObject> > CachedObjects;

    TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;

    TArray<TSharedPtr<FString>> ComboItems;

    // 初始化下拉框默认显示
    TSharedPtr<FString> InitialSelection;


};

