// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/Menu/MenuConfig.h"

#include "Config/Datatable/EditorCommandConfig.h"
#include "Framework/Command/CommandRegistry.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

void FMenuConfig::RegisterCommands()
{
	UDataTable* CommandTable = LoadObject<UDataTable>(nullptr, TEXT("/UniversalEditorModuler/Config/Datatable/MenuConfig"));
	if (!CommandTable) return;

	// 注册时通过引用传递到Map，由Map控制其生命周期
	FSlateStyleSet* MyStyleSet = new FSlateStyleSet("UEMEditor");
	FSlateStyleRegistry::RegisterSlateStyle(*MyStyleSet);
	
	for (auto& RowPair : CommandTable->GetRowMap())
	{
		FEditorCommandConfig* Config = (FEditorCommandConfig*)RowPair.Value;
		if (!Config) continue;
		
		// 注册图标
		UTexture2D* LoadedTex = Config->IconTexture.LoadSynchronous();
		if (LoadedTex)
		{
			const FVector2D IconSize(40,40);

			// FSlateImageBrush 用于注册到 StyleSet
			TSharedPtr<FSlateBrush> Brush = MakeShareable(new FSlateImageBrush(
				LoadedTex, 
				IconSize
			));

			MyStyleSet->Set<FSlateBrush>(Config->CommandTag.GetTagName(), Brush.Get());
		}

		FSlateIcon Icon(MyStyleSet->GetStyleSetName(), Config->CommandTag.GetTagName());
		
		TSharedPtr<FUICommandInfo> Cmd;
		
		FUICommandInfo::MakeCommandInfo(
		SharedThis(this),
			Cmd,
			Config->CommandTag.GetTagName(),
			Config->DisplayName,
			Config->Tooltip,
			Icon,
			EUserInterfaceActionType::Button,
			Config->Shortcut
		);
		
		FCommandRegistry::Get().RegisterCommand(Config->CommandTag, Cmd);
	}
}

#undef LOCTEXT_NAMESPACE
