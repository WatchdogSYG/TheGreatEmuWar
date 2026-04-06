// Fill out your copyright notice in the Description page of Project Settings.
#include "EchoGameInstance.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

////////////////////////////////////////////////////////////////
////  INITIALISATION
////////////////////////////////////////////////////////////////


void UEchoGameInstance::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initialising UEchoGameInstance"));
	//IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	UE_LOG(LogTemp, Display, TEXT("UEchoGameInstance Initialised"));
}

////////////////////////////////////////////////////////////////
////  ONLINE SUBSYSTEM 
////////////////////////////////////////////////////////////////

bool UEchoGameInstance::IsLoggedIn() { return false; }

FString UEchoGameInstance::GetOnlineSubsystemName(){
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem) { return OnlineSubsystem->GetSubsystemName().ToString(); }
	return TEXT("NULL");
}

////////////////////////////////////////////////////////////////
////  ONLINE SUBSYSTEM FRIENDS
////////////////////////////////////////////////////////////////

TArray<FEchoOnlineFriend> UEchoGameInstance::GetCachedFriendsList() { return CachedFriendsList; }

TArray<FEchoOnlineFriend> UEchoGameInstance::ClearCachedFriendsList()
{
	CachedFriendsList.Empty();
	return CachedFriendsList;
}

void UEchoGameInstance::LogCachedFriendsList() {
	for (auto& Friend : CachedFriendsList)
	{
		UE_LOG(LogTemp, Log, TEXT("Friend: %s (% s)"),
			*Friend.DisplayName,
			*Friend.UserId
		);
	}
}

////////////////////////////////////////////////////////////////
////  ONLINE SUBSYSTEM PROFILE
////////////////////////////////////////////////////////////////

FString UEchoGameInstance::GetUniqueNetId(int32 LocalUserNum) const {
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) {
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found"));
		return TEXT("0");
	}
	IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	FUniqueNetIdPtr MyUniqueNetId = OnlineIdentity->GetUniquePlayerId(LocalUserNum);
	return MyUniqueNetId->ToString(); 
}

FLocalProfile UEchoGameInstance::GetLocalProfile()
{
	return LocalProfile;
}

