// RecordInputNodeStyle.h
// @author : xiangyp
// @date : 2025/06/09 17:42:06
// @desc :

#pragma once
#include "DataStyle/SNNodeStyle.h"

class FSNRecordInputNodeStyle : public FSNNodeStyle
{
public:
    FSNRecordInputNodeStyle();
    virtual ~FSNRecordInputNodeStyle() override = default;

    virtual FString GetTypeName() const override;
};
