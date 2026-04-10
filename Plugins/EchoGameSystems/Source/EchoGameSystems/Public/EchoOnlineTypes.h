#pragma once

#include "CoreMinimal.h"
#include "EchoOnlineTypes.generated.h"

/*
*	Note: UHT cannot pass typedefs (in OnlineSubsystem.h),
*   so we must extract data from OnlineSubsystem types to something like a string.
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

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* ProfileImage;
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