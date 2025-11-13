#pragma once
#include "DataStyle/SNNodeStyle.h"
#include "Node/SNPerformerNode.h"

class FSNSequencerContext;
/**
 * 通用的关键帧节点样式类，用于定义右键节点有哪些行为
 * 哪个节点需要关键帧功能，就在下方注册
 */
class FSNKeyFrameNodeStyle : public FSNNodeStyle
{
public:
    FSNKeyFrameNodeStyle();

    FSNKeyFrameNodeStyle(FString InTypeName, FLinearColor InBottomColor, FLinearColor InBackgroundColor, FSlateIcon InIcon);

    virtual ~FSNKeyFrameNodeStyle() override;

    FSNSequencerContext* SNSequencerContext;
public:
    virtual void OnContextMenu(FMenuBuilder& MenuBuilder, const USNAssetBase* Asset, FSNSequencerContext* Context) override;

    int32 HasSelectKeyFrame(const USNNodeBase* Node);

    void ShowAddedKeyFrame(USNKeyFrameWrapper* Wrapper);
    void OnKeyFrameClicked(USNKeyFrameWrapper* Wrapper);
};
AUTO_REGISTER_STYLE(USNPerformerNode, FSNKeyFrameNodeStyle)
// AUTO_REGISTER_STYLE(USNXXXNode, FSNKeyFrameNodeStyle)
