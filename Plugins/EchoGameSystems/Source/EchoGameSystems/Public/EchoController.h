// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EchoController.generated.h"

/**
 * 
 */
UCLASS()
class ECHOGAMESYSTEMS_API AEchoController : public APlayerController
{
	GENERATED_BODY()
	
	

	virtual void PostSeamlessTravel() override;

public: 

	UFUNCTION(BlueprintImplementableEvent, Category = "Networking")
	void BlueprintPostSeamlessTravel();


};
