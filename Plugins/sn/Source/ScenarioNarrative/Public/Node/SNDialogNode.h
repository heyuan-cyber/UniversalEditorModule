// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DNComponent.h"
#include "SNNodeBase.h"
#include "SNDialogNode.generated.h"

class USNDialogTrack;

UCLASS()
class SCENARIONARRATIVE_API USNDialogNode : public USNNodeBase
{
    GENERATED_BODY()

public:
    USNDialogNode() : NodeIndex(-1) {}

    UPROPERTY(VisibleInstanceOnly, Category = "Setting")
    int32 NodeIndex;

    UPROPERTY(VisibleAnywhere, Category = "Data")
    FDNNodeData NodeData;

    UPROPERTY(EditAnywhere, Category = "Setting")
    bool bInterrupt;

    virtual TSet<UClass*> AttachedToTracks() const override;
};
