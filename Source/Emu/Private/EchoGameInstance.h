// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoOnlineTypes.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "EchoGameInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class EMU_API UEchoGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	
	TArray<FEchoOnlineFriend> CachedFriendsList;
	FLocalProfile LocalProfile;

	void OnReadFriendsListComplete(int32 LocalUserNum,
		bool bWasSuccessful,
		const FString& ListName,
		const FString& ErrorStr);
public:
	////////////////////////////////////////////////////////////////
	////  INITIALISATION
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable)
	void Init();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM 
	////////////////////////////////////////////////////////////////

	bool IsLoggedIn();
	FString GetOnlineSubsystemName();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM FRIENDS
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable)
	TArray<FEchoOnlineFriend> GetCachedFriendsList();

	UFUNCTION(BlueprintCallable)
	TArray<FEchoOnlineFriend> ClearCachedFriendsList();

	UFUNCTION(BlueprintCallable)
	void LogCachedFriendsList();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM PROFILE
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable)
	FString GetUniqueNetId(int32 LocalUserNum) const;

	UFUNCTION(BlueprintCallable)
	FLocalProfile GetLocalProfile();

	//UFUNCTION(BlueprintCallable)
	//void GetAllFriends();
};
