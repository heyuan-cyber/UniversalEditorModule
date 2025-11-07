// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "EditorCommandConfig.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEditorCommandGroup : uint8
{
	Menu,
	Toolbar,
	Layout
};

USTRUCT(BlueprintType)
struct FEditorCommandConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 唯一标识，使用 GameplayTag */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	FGameplayTag CommandTag;

	/** 按钮显示名字 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	FText DisplayName;

	/** 按钮提示 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	FText Tooltip;
	
	/** 图标（可以直接指定一张 Texture） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")																																																																																																																																																																																																																																																																																			
	TSoftObjectPtr<UTexture2D> IconTexture;

	/** 快捷键 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	FInputChord Shortcut;
};
