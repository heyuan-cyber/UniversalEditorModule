// EnableInputNodeStyle.h
// @author : xiangyp
// @date : 2025/06/09 17:37:19
// @desc :

#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNEnableInputNodeStyle : public FSNNodeStyle
{
public:
    FSNEnableInputNodeStyle();
    virtual ~FSNEnableInputNodeStyle() override = default;

    virtual FString GetTypeName() const override;
};
