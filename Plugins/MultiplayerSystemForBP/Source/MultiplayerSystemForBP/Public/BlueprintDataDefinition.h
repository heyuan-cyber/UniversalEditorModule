#pragma once
#include "OnlineSessionSettings.h"
//Session Delegate Declare
#include "BlueprintDataDefinition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncResultDelegate);

USTRUCT(BlueprintType)
struct FSessionPropertyKeyPair
{
	GENERATED_USTRUCT_BODY()

		FName Key;
		FVariantData Data;

};

USTRUCT(BlueprintType)
struct FSessionFlagForBool
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		FName Key;
		UPROPERTY(BlueprintReadWrite)
	    bool Value;
};
USTRUCT(BlueprintType)
struct FSessionFlagForInt
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		FName Key;
		UPROPERTY(BlueprintReadWrite)
		int Value;
};
USTRUCT(BlueprintType)
struct FSessionFlagForString
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		FName Key;
		UPROPERTY(BlueprintReadWrite)
		FString Value;
};
USTRUCT(BlueprintType)
struct FSessionFlagSettings
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<FSessionFlagForString> StringFlags;
		UPROPERTY(BlueprintReadWrite)
		TArray<FSessionFlagForInt> IntFlags;
		UPROPERTY(BlueprintReadWrite)
		TArray<FSessionFlagForBool> BoolFlags;
};
UENUM(BlueprintType)
enum class ESessionState
{
	/** An online session has not been created yet */
	NoSession,
	/** An online session is in the process of being created */
	Creating,
	/** Session has been created but the session hasn't started (pre match lobby) */
	Pending,
	/** Session has been asked to start (may take time due to communication with backend) */
	Starting,
	/** The current session has started. Sessions with join in progress disabled are no longer joinable */
	InProgress,
	/** The session is still valid, but the session is no longer being played (post match lobby) */
	Ending,
	/** The session is closed and any stats committed */
	Ended,
	/** The session is being destroyed */
	Destroying
};

