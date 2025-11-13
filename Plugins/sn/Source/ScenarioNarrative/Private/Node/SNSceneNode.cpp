// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/SNSceneNode.h"

#include "Track/SNSceneTrack.h"

TSet<UClass*> USNSceneNode::AttachedToTracks() const
{
    return
    {
        USNSceneTrack::StaticClass()
    };
}
