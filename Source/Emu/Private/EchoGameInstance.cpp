// Fill out your copyright notice in the Description page of Project Settings.
#include "EchoGameInstance.h"

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

void UEchoGameInstance::LogFriendsList() {
	for (auto& Friend : FriendsList)
	{
		UE_LOG(LogTemp, Log, TEXT("Friend: %s (% s)"),
			*Friend->GetDisplayName(),
			*Friend->GetUserId()->ToString()
		);
	}
}

////////////////////////////////////////////////////////////////
////  ONLINE SUBSYSTEM PROFILE
////////////////////////////////////////////////////////////////

FString UEchoGameInstance::GetMyUniqueNetId() const {
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) {
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found"));
		return TEXT("0");
	}
	//IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	//MyUniqueNetId = OnlineIdentity->GetUniquePlayerId(0);
	//LocalProfile.MyUniqueNetId = MyUniqueNetId->ToString();
	//LocalProfile.PlayerNickname = OnlineIdentity->GetPlayerNickname(*MyUniqueNetId);
	
	return MyUniqueNetId->ToString(); 
}

FLocalProfile UEchoGameInstance::GetLocalProfile()
{
	return LocalProfile;
}

