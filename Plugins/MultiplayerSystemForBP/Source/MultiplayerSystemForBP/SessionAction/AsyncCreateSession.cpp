// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncCreateSession.h"


UAsyncCreateSession::UAsyncCreateSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)

{
	CreateCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted);
	NumPublicConnections = 1;
}

UAsyncCreateSession* UAsyncCreateSession::AsyncCreateSession(UObject* WorldContextObject, FSessionFlagSettings SearchFlags, APlayerController* PlayerController, int32 PublicConnections, int32 PrivateConnections, bool bUseLAN, bool bAllowInvites, bool bIsDedicatedServer, bool bUsePresence, bool bUseLobbiesIfAvailable, bool bAllowJoinViaPresence, bool bAllowJoinViaPresenceFriendsOnly, bool bAntiCheatProtected, bool bUsesStats, bool bShouldAdvertise, bool bUseLobbiesVoiceChatIfAvailable, bool bStartAfterCreate)
{
	UAsyncCreateSession* Proxy = NewObject<UAsyncCreateSession>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->NumPublicConnections = PublicConnections;
	Proxy->NumPrivateConnections = PrivateConnections;
	Proxy->bUseLAN = bUseLAN;
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->bAllowInvites = bAllowInvites;
	Proxy->SearchFlags = SearchFlags;
	Proxy->bDedicatedServer = bIsDedicatedServer;
	Proxy->bUsePresence = bUsePresence;
	Proxy->bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;
	Proxy->bAllowJoinViaPresence = bAllowJoinViaPresence;
	Proxy->bAllowJoinViaPresenceFriendsOnly = bAllowJoinViaPresenceFriendsOnly;
	Proxy->bAntiCheatProtected = bAntiCheatProtected;
	Proxy->bUsesStats = bUsesStats;
	Proxy->bShouldAdvertise = bShouldAdvertise;
	Proxy->bUseLobbiesVoiceChatIfAvailable = bUseLobbiesVoiceChatIfAvailable;
	Proxy->bStartAfterCreate = bStartAfterCreate;
	return Proxy;

}

void UAsyncCreateSession::Activate()
{
	IOnlineSessionPtr SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();

	if (SessionInterface.IsValid())
	{
		CreateCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);

		FOnlineSessionSettings Setting;

		Setting.bAllowInvites = bAllowInvites;
		Setting.bAllowJoinInProgress = bAllowJoinViaPresence;
		Setting.bAllowJoinViaPresence = bAllowJoinViaPresence;
		Setting.bAllowJoinViaPresenceFriendsOnly = bAllowJoinViaPresenceFriendsOnly;
		Setting.bAntiCheatProtected = bAntiCheatProtected;
		Setting.bIsDedicated = bDedicatedServer;
		if (bDedicatedServer)
		{
			Setting.bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;
			Setting.bUsesPresence = bUsePresence;
		}
		else
		{
			Setting.bUseLobbiesIfAvailable = false;
			Setting.bUsesPresence = false;
		}
		Setting.bIsLANMatch = bUseLAN;
		Setting.bShouldAdvertise = bShouldAdvertise;
		Setting.BuildUniqueId = GetBuildUniqueId();
		Setting.bUseLobbiesVoiceChatIfAvailable = bUseLobbiesVoiceChatIfAvailable;
		Setting.bUsesStats = bUsesStats;
		Setting.bAllowInvites = bAllowInvites;


			//ExtraSettings : For FindSession through custum parameters 
			for (FSessionFlagForBool CustumSet : SearchFlags.BoolFlags)
			{
				FOnlineSessionSetting QuerySet;
				QuerySet.Data = CustumSet.Value;
				QuerySet.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Settings.Add(CustumSet.Key, QuerySet);
			}
			for (FSessionFlagForInt CustumSet : SearchFlags.IntFlags)
			{
				FOnlineSessionSetting QuerySet;
				QuerySet.Data = CustumSet.Value;
				QuerySet.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Settings.Add(CustumSet.Key, QuerySet);
			}
			for (FSessionFlagForString CustumSet : SearchFlags.StringFlags)
			{
				FOnlineSessionSetting QuerySet;
				QuerySet.Data = CustumSet.Value;
				QuerySet.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Settings.Add(CustumSet.Key, QuerySet);
			}
		

		IOnlineIdentityPtr IdentityInterface = IOnlineSubsystem::Get()->GetIdentityInterface();
		FName SessionName = NAME_GameSession;
		if (IdentityInterface.IsValid() && PlayerControllerWeakPtr.IsValid())
		{
			APlayerState* PS = PlayerControllerWeakPtr->PlayerState.Get();
			if (PS)
			{
				FUniqueNetIdPtr UserId = PS->GetUniqueId().GetUniqueNetId();
				if (UserId.IsValid())
				{
					SessionName = FName{ IdentityInterface->GetPlayerNickname(*UserId) };
					FOnlineSessionSetting QuerySet;
					QuerySet.Data = SessionName.ToString();
					QuerySet.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
					Setting.Settings.Add(TEXT("SessionName"), QuerySet);
				}
				
			}

		}

		SessionInterface->CreateSession(0, SessionName, Setting);
		return;

	}
	OnFailure.Broadcast();
}

void UAsyncCreateSession::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);
	}
	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
	}
	else
	{
		OnSuccess.Broadcast();
	}
}
