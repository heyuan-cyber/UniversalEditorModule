// SNHotkeys.h
// @author : xiangyp
// @date : 2025/06/23 17:41:55
// @desc : 快捷键支持

#pragma once
#include "Framework/Commands/Commands.h"
#include "Templates/SharedPointer.h"

class FUICommandInfo;

class FSNHotkeys : public TCommands<FSNHotkeys>
{
public:
    FSNHotkeys();
    virtual ~FSNHotkeys() override = default;

    TSharedPtr<FUICommandInfo> AddNode;
    TSharedPtr<FUICommandInfo> DeleteSNAsset;

    virtual void RegisterCommands() override;
};
