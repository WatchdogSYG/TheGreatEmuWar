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

USTRUCT(BlueprintType)
struct FLocalProfile {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString MyUniqueNetId;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerNickname;

	//FTexture2D* ProfilePicture;
};

/**
 * 
 */
UCLASS(Blueprintable)
class EMU_API UEchoGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	
	TArray<TSharedRef<FOnlineFriend>> FriendsList;
	FUniqueNetIdPtr MyUniqueNetId;
	FLocalProfile LocalProfile;
	IOnlineFriendsPtr OnlineFriends;
	IOnlineIdentityPtr OnlineIdentity;

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
	
	//IOnlineFriendsPtr AsyncReadFriendsList();

	//UFUNCTION(BlueprintCallable)
	//TArray<FEchoOnlineFriend> GetCachedFriendsList();

	UFUNCTION(BlueprintCallable)
	void LogFriendsList();

	////////////////////////////////////////////////////////////////
	////  ONLINE SUBSYSTEM PROFILE
	////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	FString GetMyUniqueNetId() const;

	UFUNCTION(BlueprintCallable)
	FLocalProfile GetLocalProfile();

	//UFUNCTION(BlueprintCallable)
	//void GetAllFriends();
};
