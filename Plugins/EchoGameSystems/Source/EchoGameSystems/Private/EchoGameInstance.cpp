// Fill out your copyright notice in the Description page of Project Settings.
#include "EchoGameInstance.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

////////////////////////////////////////////////////////////////
////  INITIALISATION
////////////////////////////////////////////////////////////////


void UEchoGameInstance::Init() {

	Super::Init();

	UE_LOG(LogTemp, Display, TEXT("Initialising UEchoGameInstance"));

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem not found"));
		return;
	}

	IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	if (!OnlineIdentity.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineIdentity interface invalid"));
		return;
	}

	// Bind login complete
	OnlineIdentity->OnLoginCompleteDelegates->AddUObject(
		this,
		&UEchoGameInstance::OnLoginCompleteDelegate
	);

	// Bind login status changed
	OnlineIdentity->OnLoginStatusChangedDelegates->AddUObject(
		this,
		&UEchoGameInstance::OnLoginStatusChangedDelegate
	);

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

ELoginStatusType UEchoGameInstance::GetLoginStatus() { return LoginStatus; }

////////////////////////////////////////////////////////////////
////  ONLINE SUBSYSTEM FRIENDS
////////////////////////////////////////////////////////////////

TArray<FEchoOnlineFriend> UEchoGameInstance::GetCachedFriendsList() { return CachedFriendsList; }

TArray<FEchoOnlineFriend> UEchoGameInstance::SetCachedFriendsList(TArray<FEchoOnlineFriend> InFriendsList)
{
	CachedFriendsList = InFriendsList;
	return CachedFriendsList;
}

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
////  ONLINE SUBSYSTEM IDENTITY
////////////////////////////////////////////////////////////////

ELoginStatusType UEchoGameInstance::ConvertLoginType(ELoginStatus::Type InStatus)
{
	switch (InStatus)
	{
	case ELoginStatus::NotLoggedIn:
		return ELoginStatusType::NotLoggedIn;
	case ELoginStatus::UsingLocalProfile:
		return ELoginStatusType::UsingLocalProfile;
	case ELoginStatus::LoggedIn:
		return ELoginStatusType::LoggedIn;
	}
	return ELoginStatusType::NotLoggedIn;
}

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

void UEchoGameInstance::OnLoginCompleteDelegate(
	int32 LocalUserNum,
	bool bWasSuccessful,
	const FUniqueNetId& UserId,
	const FString& Error)
{
	LoginStatus = ELoginStatusType::LoggedIn;
	OnLoginComplete();

	UE_LOG(LogTemp, Warning, TEXT("Steam Login Complete: %d, Success: %d, User: %s, Error: %s"),
		LocalUserNum,
		bWasSuccessful,
		*UserId.ToString(),
		*Error
	);
}

void UEchoGameInstance::OnLoginStatusChangedDelegate(int32 LocalUserNum,
	ELoginStatus::Type OldStatus, 
	ELoginStatus::Type NewStatus, 
	const FUniqueNetId& UserId)
{
	LoginStatus = ConvertLoginType(NewStatus);
	OnLoginStatusChanged();

	UE_LOG(LogTemp, Warning, TEXT("Steam Login Status Changed: %d -> %d for user %s"),
		(int32)OldStatus,
		(int32)NewStatus,
		*UserId.ToString()
	);
}
