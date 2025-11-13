// SNAssetStyleRegistry.h
// @author : xiangyp
// @date : 2025/04/22 17:11:08
// @desc :

#pragma once

class FSNTrackStyle;
class FSNNodeStyle;
class USNAssetBase;
class USNTrackBase;
class FSNAssetStyleBase;
class USNNodeBase;

class FSNAssetStyleRegistry
{
public:
    FSNAssetStyleRegistry() = default;

    static FSNAssetStyleRegistry& Get();

    void Register(const UClass* SNAssetClass, FSNAssetStyleBase* Style);
    template <class T = USNAssetBase>
    void Register(FSNAssetStyleBase* Style);

    template <class T = USNNodeBase>
    FSNNodeStyle* GetNodeStyle() const;

    FSNNodeStyle* GetNodeStyle(const UClass* NodeClass) const;

    template <class T = USNTrackBase>
    FSNTrackStyle* GetTrackStyle() const;

    FSNTrackStyle* GetTrackStyle(const UClass* TrackClass) const;

    void Clear();

public:
    TMap<const UClass*, FSNAssetStyleBase*> RegisteredStyle;

    static FSNNodeStyle* DefaultNodeStyle;
    static FSNTrackStyle* DefaultTrackStyle;
};
