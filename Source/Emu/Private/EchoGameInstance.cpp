// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoGameInstance.h"

TArray<FEchoOnlineFriend> UEchoGameInstance::GetFriendsList() {
	
	TArray<FEchoOnlineFriend> CachedFriendsList;
	for (auto& Friend : FriendsList) {
		FEchoOnlineFriend F;
		F.DisplayName = Friend->GetDisplayName();
		F.UserId = Friend->GetUserId()->ToString();
		F.Presence = Friend->GetPresence().Status.StatusStr;
		CachedFriendsList.Add(F);
	}
	return CachedFriendsList;
}

void UEchoGameInstance::LogFriendsList() {
	for (auto& Friend : FriendsList)
	{
		UE_LOG(LogTemp, Log, TEXT("Friend: %s (% s)"),
			*Friend->GetDisplayName(),
			*Friend->GetUserId()->ToString()
		);
	}
}

void UEchoGameInstance::Init() {
	OnlineSubsystem = IOnlineSubsystem::Get();

	if (!OnlineSubsystem) {
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found"));
		return;
	}

	OnlineFriends = OnlineSubsystem->GetFriendsInterface();

	if (!OnlineFriends) {
		UE_LOG(LogTemp, Warning, TEXT("No OnlineFriends found"));
		return;
	}

	OnlineFriends->ReadFriendsList(
		0,
		TEXT("default"), //must be default friends list in current OnlineSubsystemSteam implementation (as opposed to blocked or recent
		FOnReadFriendsListComplete::CreateUObject(this, &UEchoGameInstance::OnReadFriendsListComplete)
	);
}

void UEchoGameInstance::OnReadFriendsListComplete(
	int32 LocalUserNum,
	bool bWasSuccessful,
	const FString& ListName,
	const FString& ErrorStr) {
	
	if (!bWasSuccessful) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to read Steam friends: %s"), *ErrorStr);
		return;
	}

	OnlineFriends->GetFriendsList(LocalUserNum, ListName, FriendsList);
	}


