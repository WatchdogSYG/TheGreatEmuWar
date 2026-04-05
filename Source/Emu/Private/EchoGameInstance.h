// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "EchoGameInstance.generated.h"


/*
*	UHT cannot pass TypeDefs (in .h), so we must extract data from OnlineSubsystem types to something like a string.
*/
USTRUCT(BlueprintType)
struct FEchoOnlineFriend {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly)
	FString UserId;

	UPROPERTY(BlueprintReadOnly)
	FString Presence;
};

/**
 * 
 */
UCLASS(Blueprintable)
class EMU_API UEchoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	IOnlineSubsystem* OnlineSubsystem;
	IOnlineFriendsPtr OnlineFriends;
	IOnlineIdentityPtr OnlineIdentity;
	TArray<TSharedRef<FOnlineFriend>> FriendsList;

	UFUNCTION(BlueprintCallable)
	void Init();
	void OnReadFriendsListComplete(int32 LocalUserNum,
		bool bWasSuccessful,
		const FString& ListName,
		const FString& ErrorStr);
		
	UFUNCTION(BlueprintCallable)
	 TArray<FEchoOnlineFriend> GetFriendsList();

	UFUNCTION(BlueprintCallable)
	void LogFriendsList();
};
