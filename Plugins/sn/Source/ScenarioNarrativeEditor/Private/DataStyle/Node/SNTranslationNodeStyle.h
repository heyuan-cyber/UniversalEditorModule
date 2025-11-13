// TranslationNodeStyle.h
// @author : xiangyp
// @date : 2025/06/09 17:44:03
// @desc :

#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNTranslationNodeStyle : public FSNNodeStyle
{
public:
    FSNTranslationNodeStyle();
    virtual ~FSNTranslationNodeStyle() override = default;

    virtual FString GetTypeName() const override;
};
