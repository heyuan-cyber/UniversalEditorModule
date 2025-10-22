// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

class UEMEDITOR_API FMenuConfig : public TCommands<FMenuConfig>
{
public:
	FMenuConfig()
		: TCommands<FMenuConfig>(
			TEXT("UEMEditor"), // Context name
			NSLOCTEXT("Contexts", "UEMEditor", "UEM Editor"),
			NAME_None,
			FAppStyle::GetAppStyleSetName()
		)
	{}
	
	virtual void RegisterCommands() override;
};
