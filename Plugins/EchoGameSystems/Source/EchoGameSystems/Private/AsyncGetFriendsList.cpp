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

		if (Friend->GetPresence().bIsOnline) {
			F.Presence = TEXT("Online");
		}
		else {
			F.Presence = TEXT("Offline");
		}

		/*
		CSteamID SteamID((uint64)Friend->GetUserId()->GetBytes());
		int ImageId = SteamFriends()->GetLargeFriendAvatar(SteamID);
		uint32 Width, Height;
		if (SteamUtils()->GetImageSize(ImageId, &Width, &Height))
		{
			TArray<uint8> RawRGBA;
			RawRGBA.SetNum(Width * Height * 4);

			SteamUtils()->GetImageRGBA(ImageId, RawRGBA.GetData(), RawRGBA.Num());
		}
		*/
		ReturnValue.Add(F);
	}
	OnSuccess.Broadcast(ReturnValue);
}
/*
UTexture2D* UAsyncGetFriendsList::CreateTextureFromRGBA(const TArray<uint8>& RawRGBA, uint32 Width, uint32 Height)
{
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
	if (!Texture) return nullptr;

	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, RawRGBA.GetData(), RawRGBA.Num());
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	Texture->UpdateResource();

	return Texture;
}
*/