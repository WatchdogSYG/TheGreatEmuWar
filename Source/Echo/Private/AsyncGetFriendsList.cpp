// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncGetFriendsList.h"

UAsyncGetFriendsList* UAsyncGetFriendsList::ReadFriendsList(UObject* WorldContextObject)
{
	UAsyncGetFriendsList* Node = NewObject< UAsyncGetFriendsList>();
	Node->WorldContextObject = WorldContextObject;
	return Node;
}

void UAsyncGetFriendsList::Activate()
{
	 OnlineSubsystem = IOnlineSubsystem::Get();

	if (!OnlineSubsystem) {
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found"));
		OnFailure.Broadcast({});
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("OnlineSubsystem Loaded"));

	OnlineFriends = OnlineSubsystem->GetFriendsInterface();
	if (!OnlineFriends) {
		UE_LOG(LogTemp, Warning, TEXT("No OnlineFriends found"));
		OnFailure.Broadcast({});
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("OnlineFriends Loaded"));
	OnlineFriends->ReadFriendsList(
		0,
		TEXT("default"), //must be default friends list in current OnlineSubsystemSteam implementation (as opposed to blocked or recent
		FOnReadFriendsListComplete::CreateUObject(this, &UAsyncGetFriendsList::OnReadFriendsListComplete)
	);
}

void UAsyncGetFriendsList::OnReadFriendsListComplete(
	int32 LocalUserNum,
	bool bWasSuccessful,
	const FString& ListName,
	const FString& ErrorStr) {

	if (!bWasSuccessful) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to read Steam friends: %s"), *ErrorStr);
		OnFailure.Broadcast({});
		return;
	}
	
	TArray<FEchoOnlineFriend> ReturnValue;

	//at this point it is highly likely (guaranteed?) that cached OnlineSubsystem and OnlineFriends are valid pointers.
	TArray<TSharedRef<FOnlineFriend>> FriendsList;
	OnlineFriends->GetFriendsList(LocalUserNum, ListName, FriendsList);

	for (auto& Friend : FriendsList) {
		FEchoOnlineFriend F;
		F.DisplayName = Friend->GetDisplayName();
		F.UserId = Friend->GetUserId()->ToString();
		F.Presence = Friend->GetPresence().Status.StatusStr;
		ReturnValue.Add(F);
	}
	OnSuccess.Broadcast(ReturnValue);
}
