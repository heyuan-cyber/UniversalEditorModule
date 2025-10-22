// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"

/**
 * 
 */


class UEMEDITOR_API FCommandRegistry
{
private:
	TMap<FGameplayTag, TSharedPtr<FUICommandInfo>> Commands;
	
public:
	static FCommandRegistry& Get()
	{
		static FCommandRegistry Instance;
		return Instance;
	}

	/** 注册命令，用 GameplayTag 做 Key */
	void RegisterCommand(FGameplayTag Tag, TSharedPtr<FUICommandInfo> Command)
	{
		Commands.Add(Tag, Command);
	}
	
	/** 获取命令 */
	TSharedPtr<FUICommandInfo> GetCommand(FGameplayTag Tag) const
	{
		const TSharedPtr<FUICommandInfo>* Found = Commands.Find(Tag);
		return Found ? *Found : nullptr;
	}
};
