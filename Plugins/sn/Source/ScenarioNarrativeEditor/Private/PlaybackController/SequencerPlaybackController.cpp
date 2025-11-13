// SequencerPlaybackController.cpp
// @author : gongpx
// @date : 2025/08/13 16:48:59
// @desc :
#include "SequencerPlaybackController.h"

#include "SNSequencerContext.h"

#include "SequencerPlaybackController.h"
#include "SNSequencerContext.h"

FSequencerPlaybackController::FSequencerPlaybackController(FSNSequencerContext* InContext)
    : Context(InContext)
{

}

void FSequencerPlaybackController::Play()
{
    if (PlaybackMode == ESequencerPlaybackMode::Stopped || PlaybackMode == ESequencerPlaybackMode::Paused)
    {
        SequenceDuration = Context->GetMaxTrackTime();
        PlaybackMode = ESequencerPlaybackMode::Playing;
        OnPlaybackStateChanged.Broadcast();
    }
}

void FSequencerPlaybackController::Pause()
{
    if (PlaybackMode == ESequencerPlaybackMode::Playing)
    {
        PlaybackMode = ESequencerPlaybackMode::Paused;
        OnPlaybackStateChanged.Broadcast();
    }
}

void FSequencerPlaybackController::Stop()
{
    if (PlaybackMode != ESequencerPlaybackMode::Stopped)
    {
        PlaybackMode = ESequencerPlaybackMode::Stopped;
        // 重置到起始位置
        SetCurrentTime(0.0f);
        OnPlaybackStateChanged.Broadcast();
    }
}

void FSequencerPlaybackController::SetCurrentTime(float NewTime)
{
    // 处理循环逻辑
    if (bLooping && SequenceDuration > 0)
    {
        if (NewTime > SequenceDuration)
        {
            NewTime = FMath::Fmod(NewTime, SequenceDuration);
        }
        else if (NewTime < 0)
        {
            NewTime = SequenceDuration + FMath::Fmod(NewTime, SequenceDuration);
        }
    }

    // 限制在有效范围内
    NewTime = FMath::Clamp(NewTime, 0.0f, SequenceDuration);

    if (CurrentTime != NewTime)
    {
        CurrentTime = NewTime;

        // 关键点：调用上下文的ChangeEditorCurrentTime更新预览场景
        if (Context)
        {
            Context->ChangeEditorCurrentTime(CurrentTime);
        }

        // 广播时间改变事件
        OnTimeChanged.Broadcast(CurrentTime);
    }
}

void FSequencerPlaybackController::StepToNextFrame()
{
    const float FrameDuration = 1.0f / FrameRate;
    SetCurrentTime(CurrentTime + FrameDuration);

    // 如果是停止状态，切换到暂停状态
    if (IsStopped())
    {
        PlaybackMode = ESequencerPlaybackMode::Paused;
        OnPlaybackStateChanged.Broadcast();
    }
}

void FSequencerPlaybackController::StepToPreviousFrame()
{
    const float FrameDuration = 1.0f / FrameRate;
    SetCurrentTime(CurrentTime - FrameDuration);

    // 如果是停止状态，切换到暂停状态
    if (IsStopped())
    {
        PlaybackMode = ESequencerPlaybackMode::Paused;
        OnPlaybackStateChanged.Broadcast();
    }
}

void FSequencerPlaybackController::SetLooping(bool newbLooping)
{
    if (bLooping != newbLooping)
    {
        bLooping = newbLooping;
        OnPlaybackStateChanged.Broadcast();
    }
}

void FSequencerPlaybackController::Tick(float DeltaTime)
{
    if (IsPlaying())
    {
        // 推进时间
        SetCurrentTime(CurrentTime + DeltaTime);

        // 检查是否到达结尾（循环逻辑在SetCurrentTime中处理）
        if (!bLooping && CurrentTime >= SequenceDuration)
        {
            Stop();
        }
    }
}
