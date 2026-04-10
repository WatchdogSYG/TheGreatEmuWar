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


UENUM(BlueprintType)
enum class ELoginStatusType : uint8
{
	/** Player has not logged in or chosen a local profile */
	NotLoggedIn,
	/** Player is using a local profile but is not logged in */
	UsingLocalProfile,
	/** Player has been validated by the platform specific authentication service */
	LoggedIn
};

/**
 * 
 */
UCLASS(Blueprintable)
class ECHOGAMESYSTEMS_API UEchoGameInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	////////////////////////////////////////////////////////////////
	////  INITIALISATION
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "Initialisation")
	void Init();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM 
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystem")
	bool IsLoggedIn();
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystem")
	FString GetOnlineSubsystemName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "OnlineSubsystem")
	ELoginStatusType GetLoginStatus();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM FRIENDS
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystemFriends")
	TArray<FEchoOnlineFriend> GetCachedFriendsList();

	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystemFriends")
	TArray<FEchoOnlineFriend> SetCachedFriendsList(TArray<FEchoOnlineFriend> InFriendsList);

	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystemFriends")
	TArray<FEchoOnlineFriend> ClearCachedFriendsList();

	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystemFriends")
	void LogCachedFriendsList();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM IDENTITY
	////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystemIdentity")
	FString GetUniqueNetId(int32 LocalUserNum) const;

	UFUNCTION(BlueprintCallable, Category = "OnlineSubsystemIdentity")
	FLocalProfile GetLocalProfile();

	UFUNCTION(BlueprintImplementableEvent, Category = "OnlineSubsystemIdentity")
	void OnLoginComplete();

	UFUNCTION(BlueprintImplementableEvent, Category = "OnlineSubsystemIdentity")
	void OnLoginStatusChanged();

private:

	TArray<FEchoOnlineFriend> CachedFriendsList;
	FLocalProfile LocalProfile;

	void OnReadFriendsListComplete(
		int32 LocalUserNum,
		bool bWasSuccessful,
		const FString& ListName,
		const FString& ErrorStr);

	void OnLoginCompleteDelegate(
		int32 LocalUserNum,
		bool bWasSuccessful,
		const FUniqueNetId& UserId,
		const FString& Error);

	void OnLoginStatusChangedDelegate(
		int32 LocalUserNum,
		ELoginStatus::Type OldStatus,
		ELoginStatus::Type NewStatus,
		const FUniqueNetId& UserId);

	UPROPERTY(VisibleAnywhere, Category = "OnlineSubsystemIdentity")
	ELoginStatusType LoginStatus = ELoginStatusType::NotLoggedIn;

	ELoginStatusType ConvertLoginType(ELoginStatus::Type InStatus);
};