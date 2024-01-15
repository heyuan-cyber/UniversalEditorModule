// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Kismet/BlueprintAsyncActionBase.h"
#include"../Public/BlueprintWrapperForSession.h"
#include "AsyncCreateSession.generated.h"



/**
 * 
 */
UCLASS()
class MULTIPLAYERSYSTEMFORBP_API  UAsyncCreateSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
		UPROPERTY(BlueprintAssignable)
			FAsyncResultDelegate OnSuccess;

		UPROPERTY(BlueprintAssignable)
			FAsyncResultDelegate OnFailure;


		UAsyncCreateSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


		UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|AsnycSessions")
		static UAsyncCreateSession* AsyncCreateSession(
				UObject* WorldContextObject,
				//const TArray<FSessionPropertyKeyPair>& ExtraSettings,
			    FSessionFlagSettings SearchFlagSetting,
				class APlayerController* PlayerController = NULL, 
				int32 PublicConnections = 2, 
				int32 PrivateConnections = 0, 
				bool bUseLAN = false, 
				bool bAllowInvites = true, 
				bool bIsDedicatedServer = false, 
				bool bUsePresence = true, 
				bool bUseLobbiesIfAvailable = true, 
				bool bAllowJoinViaPresence = true, 
				bool bAllowJoinViaPresenceFriendsOnly = false, 
				bool bAntiCheatProtected = false, 
				bool bUsesStats = false, 
				bool bShouldAdvertise = true, 
				bool bUseLobbiesVoiceChatIfAvailable = false, 
				bool bStartAfterCreate = true);


		virtual void Activate() override;
		// End of UOnlineBlueprintCallProxyBase interface




private:

		// Internal callback when session creation completes, optionally calls StartSession
		void OnCreateCompleted(FName SessionName, bool bWasSuccessful);

		FOnCreateSessionCompleteDelegate CreateCompleteDelegate;

		// Handles to the registered delegates above
		FDelegateHandle CreateCompleteDelegateHandle;

		// The player controller triggering things
		TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

		// Number of public connections
		int NumPublicConnections;

		// Number of private connections
		int NumPrivateConnections;

		// Whether or not to search LAN
		bool bUseLAN;

		// Whether or not to allow invites
		bool bAllowInvites;

		// Whether this is a dedicated server or not
		bool bDedicatedServer;

		// Whether to use the presence option
		bool bUsePresence;

		// Whether to prefer the use of lobbies for hosting if the api supports them
		bool bUseLobbiesIfAvailable;

		// Whether to allow joining via presence
		bool bAllowJoinViaPresence;

		// Allow joining via presence for friends only
		bool bAllowJoinViaPresenceFriendsOnly;

		// Delcare the server to be anti cheat protected
		bool bAntiCheatProtected;

		// Record Stats
		bool bUsesStats;

		// Should advertise server?
		bool bShouldAdvertise;

		// Whether to prefer the use of voice chat lobbies if the api supports them
		bool bUseLobbiesVoiceChatIfAvailable;

		// Whether to start the session automatically after it is created
		bool bStartAfterCreate;

		// Store extra settings
		TArray<FSessionPropertyKeyPair> ExtraSettings;

		// The world context object in which this call is taking place
		UObject* WorldContextObject;

		FSessionFlagSettings SearchFlags;
};
