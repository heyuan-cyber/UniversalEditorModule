#include "Preview/SNPreviewSpline.h"

#include "Components/BoxComponent.h"

ASNPreviewSpline::ASNPreviewSpline()
{
    PrimaryActorTick.bCanEverTick = true;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;
}

void ASNPreviewSpline::MoveAlongSpline(UCameraComponent* SplineCameraComponent, float Distance)
{
    CurrentDistance = FMath::Clamp(Distance, 0.0f, GetSplineLength());

    // 获取样条线上的位置和旋转
    FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
    FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

    // 设置相机位置和旋转
    SplineCameraComponent->SetWorldLocation(Location);
    SplineCameraComponent->SetWorldRotation(Rotation);

    Rotation.Pitch = -20.0f; // 向下倾斜20度
    SplineCameraComponent->SetWorldRotation(Rotation);
}

void ASNPreviewSpline::SetSplinePoints(const TArray<FVector>& Points)
{
    SplineComponent->ClearSplinePoints();
    for (const FVector& Point : Points)
    {
        SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::Local, true);
    }
    SplineComponent->UpdateSpline();
}

TArray<FVector> ASNPreviewSpline::GetSplinePoints() const
{
    TArray<FVector> Points;
    const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
    for (int32 i = 0; i < NumPoints; i++)
    {
        Points.Add(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local));
    }
    return Points;
}

void ASNPreviewSpline::AddSplinePoint(const FVector& Point)
{
    SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::Local, true);
    SplineComponent->UpdateSpline();
}
