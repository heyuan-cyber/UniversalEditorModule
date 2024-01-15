// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
//#include "BlueprintDataDefinition.h"
#include "PS_Session.generated.h"

/**
 * 
 */


UCLASS()
class MULTIPLAYERSYSTEMFORBP_API APS_Session : public APlayerState
{
	GENERATED_BODY()
	




#pragma region UserInformation
public:
	//
	//UPROPERTY(Replicated, Category = Session)
	//bool bHoster = false;



#pragma endregion

#pragma region SessionInformation
public:
	//UPROPERTY(Replicated, Category = Session)
	//ESessionState SessionState = ESessionState::NoSession;
#pragma endregion



};
