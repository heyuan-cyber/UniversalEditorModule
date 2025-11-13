// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/SNDialogNode.h"
#include "Track/SNDialogTrack.h"

TSet<UClass*> USNDialogNode::AttachedToTracks() const
{
    return
    {
        USNDialogTrack::StaticClass()
    };
}
