// AnimationNodeDrawer.h
// @author : xiangyp
// @date : 2025/05/09 13:57:37
// @desc :

#pragma once
#include "SNCrossableNodeDrawer.h"

struct FSNNodeBlend;
class USNAnimationNode;

class FSNAnimationNodeDrawer : public FSNCrossableNodeDrawer
{
public:
    FSNAnimationNodeDrawer() = default;
    virtual ~FSNAnimationNodeDrawer() override = default;
};
