// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoOnlineTypes.h"
#include "steam/steam_api.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncGetFriendsList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendsRead, const TArray<FEchoOnlineFriend>&, OutFriendsList);

/**
 * Create a Latent BP Node to fetch FriendsList from an OnlineSubsystem.
 * src: https://www.youtube.com/watch?v=QmxuP3zCbHs
 */
UCLASS()
class ECHOGAMESYSTEMS_API UAsyncGetFriendsList : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
    UPROPERTY(BlueprintAssignable)
    FOnFriendsRead OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FOnFriendsRead OnFailure;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
    static UAsyncGetFriendsList* ReadFriendsList(UObject* WorldContextObject);

    virtual void Activate() override;

private:

    IOnlineSubsystem* OnlineSubsystem;
    IOnlineFriendsPtr OnlineFriends;

    UObject* WorldContextObject;

    void OnReadFriendsListComplete(int32 LocalUserNum,
        bool bWasSuccessful,
        const FString& ListName,
        const FString& ErrorStr);

   // UTexture2D* CreateTextureFromRGBA(const TArray<uint8>& RawRGBA, uint32 Width, uint32 Height);
   };
