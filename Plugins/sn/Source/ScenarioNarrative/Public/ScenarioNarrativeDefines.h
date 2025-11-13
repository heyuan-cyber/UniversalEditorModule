#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogScenarioNarrative, Log, All);

static constexpr float SN_FRAME_RATE = 30.f;
static constexpr float SN_ONE_FRAME_TIME = 1.f / SN_FRAME_RATE;

UENUM()
namespace ESNTraceShapeType
{
    enum Type : uint8
    {
        Line,
        Box,
        Sphere,
        Capsule
    };
}
