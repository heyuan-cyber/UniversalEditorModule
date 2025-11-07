// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

class UMEEDITOR_API FMenuConfig : public TCommands<FMenuConfig>
{
public:
	FMenuConfig()
		: TCommands<FMenuConfig>(
			TEXT("UMEEditor"), // Context name
			NSLOCTEXT("Contexts", "UMEEditor", "UME Editor"),
			NAME_None,
			FAppStyle::GetAppStyleSetName()
		)
	{}
	
	virtual void RegisterCommands() override;
};
