#pragma once

#include "CoreMinimal.h"
#include "SNPreviewCamera.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "SNPreviewSpline.generated.h"

UCLASS()
class SCENARIONARRATIVEEDITOR_API ASNPreviewSpline : public AActor
{
    GENERATED_BODY()

public:
    ASNPreviewSpline();

    // 样条线组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Camera")
    USplineComponent* SplineComponent;

    // 沿样条线移动相机
    UFUNCTION(BlueprintCallable, Category = "Spline Camera")
    void MoveAlongSpline(UCameraComponent* SplineCameraComponent, float Distance);

    // 获取当前移动距离
    UFUNCTION(BlueprintPure, Category = "Spline Camera")
    float GetCurrentDistance() const { return CurrentDistance; }

    // 获取样条线长度
    UFUNCTION(BlueprintPure, Category = "Spline Camera")
    float GetSplineLength() const { return SplineComponent->GetSplineLength(); }

    // 预览场景专用功能：设置样条点
    void SetSplinePoints(const TArray<FVector>& Points);

    // 预览场景专用功能：获取样条点
    TArray<FVector> GetSplinePoints() const;

    // 预览场景专用功能：添加样条点
    void AddSplinePoint(const FVector& Point);

private:
    float CurrentDistance = 0.0f;
};
