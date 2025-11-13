// SequencerPlaybackController.h
// @author : gongpx
// @date : 2025/08/13 16:48:59
// @desc :

#pragma once

#include "CoreMinimal.h"

// 播放模式枚举
enum class ESequencerPlaybackMode : uint8
{
    Stopped,
    Playing,
    Paused
};

class FSNSequencerContext; // 前向声明

class FSequencerPlaybackController
{
public:
    // 修改构造函数以接收上下文
    FSequencerPlaybackController(FSNSequencerContext* InContext);

    // 播放控制
    void Play();
    void Pause();
    void Stop();

    // 帧控制
    void StepToNextFrame();
    void StepToPreviousFrame();

    // 循环控制
    void SetLooping(bool newbLooping);
    bool IsLooping() const { return bLooping; }

    // 状态查询
    bool IsPlaying() const { return PlaybackMode == ESequencerPlaybackMode::Playing; }
    bool IsPaused() const { return PlaybackMode == ESequencerPlaybackMode::Paused; }
    bool IsStopped() const { return PlaybackMode == ESequencerPlaybackMode::Stopped; }

    // 时间控制
    float GetCurrentTime() const { return CurrentTime; }
    void SetCurrentTime(float NewTime);

    // 序列长度
    float GetSequenceDuration() const { return SequenceDuration; }
    void SetSequenceDuration(float Duration) { SequenceDuration = Duration; }

    // 帧率控制
    float GetFrameRate() const { return FrameRate; }
    void SetFrameRate(float Rate) { FrameRate = Rate; }

    void Tick(float DeltaTime);

    // 播放状态改变事件
    DECLARE_MULTICAST_DELEGATE(FOnPlaybackStateChanged);
    FOnPlaybackStateChanged OnPlaybackStateChanged;

    // 时间改变事件
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float);
    FOnTimeChanged OnTimeChanged;

private:
    // 播放状态变量
    ESequencerPlaybackMode PlaybackMode = ESequencerPlaybackMode::Stopped;
    float CurrentTime = 0.0f;
    float FrameRate = 30.0f; // 默认30帧
    float SequenceDuration = 0.0f;
    bool bLooping = false;

    // 上下文引用 - 用于更新预览场景
    FSNSequencerContext* Context = nullptr;
};
