// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Containers/Queue.h"
#include "Kismet/KismetMathLibrary.h"

#include <string>

////////////////////////////////////////////////////////////////
//  TODO
////////////////////////////////////////////////////////////////

// - provide bp controls for colour, text format, size, opcity, enablement, other options (for HP/MP/SP bars)
// - threshold for linear/reduced animation time for trickle damage
// - set appropriate UPROPERTY assignments

////////////////////////////////////////////////////////////////
//  CONSTRUCTORS
////////////////////////////////////////////////////////////////

//this is how to construct a derived class from UUserWidget which has no default constructor (initialisation lists did not work)
//https://www.reddit.com/r/unrealengine/comments/ibtr4m/lkn2019_error_when_compiling_a_uuserwidget/

//https://allarsblog.com/2020/01/29/ue4-umg-uuserwidget-which-fires-first-native_preconstruct-or-blueprint-preconstruct/

void UResourceBar::NativeOnInitialized() {
	Super::NativeOnInitialized();
	//UE_LOG(LogTemp, Warning, TEXT("NativeOnInitialized Begin"));
	//set options
	Mode = TextUpdateMode::CONTINUOUS;
	DisplayMaxValue = true;

	//set defaults to avoid nullptr
	MinValue = 0.f;
	MaxValue = 69.f;

	CurrentPct = 0.01f;
	OriginalPct = CurrentPct;
	TargetPct = CurrentPct;

	InitialRefresh(CurrentPct * MaxValue, MaxValue);

	SecondaryOriginalPct = CurrentPct;
	SecondaryCurrentPct = CurrentPct;
	SecondaryTargetPct = CurrentPct;

	CurrentValue = CurrentPct * MaxValue;

	AnimationDuration = 0.2f;
	SecondaryBarDelay = 0.5f;

	MicroEventMagnitudeThreshold = 0.005f;
	MicroEventAnimationDuration = 0.01f;

	ConfigurationChanged = true;	//must be true when initialised so that the first tick knows to can assign a bar to FrontBar and BackBar
	DownConfiguration = true;

	MainColor = FColor::FromHex("#00B63A");

	UpColor = FColor::FromHex("#E1BE00FF");
	DownColor = FColor::FromHex("#D00000FF");

	//UpColor = FColor::FromHex("#6BCB5644");
	//DownColor = FColor::FromHex("#35AF1944");

	UpdateText();
	//UE_LOG(LogTemp, Warning, TEXT("UIStatbar Init Complete"));

}


////////////////////////////////////////////////////////////////
//  ENGINE FUNCTIONS
////////////////////////////////////////////////////////////////

void UResourceBar::NativeTick(const FGeometry& MyGeometry, float DeltaTime) {
	Super::NativeTick(MyGeometry, DeltaTime);

	//----------------------------------------------------------------
	//Firstly, see if enough time has passed to input a ResourceEvent into the secondary Bar
	if (PrimaryHistoryQueue.Peek() != nullptr) {
		if (GetWorld()->GetTimeSeconds() - PrimaryHistoryQueue.Peek()->EventTime > SecondaryBarDelay) {
			ResourceEvent event;
			if (PrimaryHistoryQueue.Dequeue(event)) {
				SetProgress(SecondaryBar, event.SetPct);

				//SecondaryOriginalPct = SecondaryCurrentPct;

				//RemainingSecondaryAnimationTime = AnimationDuration;

				//do we need to adjust for the inter-tick delta between (GetWorld()->GetTimeSeconds() - HitQueue.Peek()->EventTime) and (SecondaryBarDelay)? Not for now.
			}
			else {
				//we deqd nothing! This should never happen due to the peek() check
			}
		}
	}
	else {
		//nothing in the queue to peek();
	}

	//----------------------------------------------------------------
	//Secondly, increment the timers
	RemainingAnimationTime -= DeltaTime;
	RemainingSecondaryAnimationTime -= DeltaTime;//do we do this every tick?

	//----------------------------------------------------------------
	//Thirdly, configure the bars
	//Check if the config changed first before the next if..else so that we only format the bars when needed instead of every tick

	/*
	if (CurrentPct <= SecondaryCurrentPct) {// DownConfiguration
		if (!DownConfiguration) {
			DownConfiguration = true;
			ConfigurationChanged = true;
		}
	} else {								// !DownConfiguration
		if (DownConfiguration) {
			DownConfiguration = false;
			ConfigurationChanged = true;
		}
	}

	//if the configuration changed, configure the bars
	if (ConfigurationChanged) {
		if (CurrentPct <= SecondaryCurrentPct) {
			FrontBar = PrimaryBar;
			BackBar = SecondaryBar;

			FrontBar->SetFillColorAndOpacity(MainColor);
			BackBar->SetFillColorAndOpacity(DownColor);
		} else {
			FrontBar = SecondaryBar;
			BackBar = PrimaryBar;

			FrontBar->SetFillColorAndOpacity(UpColor);
			BackBar->SetFillColorAndOpacity(MainColor);
		}
	}

	ConfigurationChanged = false; //reset the configuration flag for the next tick
	*/

	FrontBar = PrimaryBar;
	BackBar = SecondaryBar;

	//----------------------------------------------------------------
	//Fourthly, interpolate the values if needed and process the DOTQueue
	if (CurrentPct != TargetPct) {
		CurrentPct = InterpolateProgress(FrontBar, OriginalPct, TargetPct, RemainingAnimationTime, AnimationDuration);
		CurrentValue = CurrentPct * MaxValue;
	}
	else {
		OriginalPct = TargetPct;
	}

	if (SecondaryCurrentPct != SecondaryTargetPct) {
		SecondaryCurrentPct = InterpolateProgress(BackBar, SecondaryOriginalPct, SecondaryTargetPct, RemainingSecondaryAnimationTime, AnimationDuration);
		SecondaryCurrentValue = SecondaryCurrentPct * MaxValue;
	}
	else {
		SecondaryOriginalPct = SecondaryTargetPct;
	}

	PrimaryHistoryQueue.Enqueue(ResourceEvent{ CurrentPct, (float)GetWorld()->GetTimeSeconds() });

	//----------------------------------------------------------------
	//Finally, update the displayed text if required
	if (Mode == TextUpdateMode::CONTINUOUS) {
		UpdateText();
	}
}

////////////////////////////////////////////////////////////////
//  PUBLIC INTERFACE
////////////////////////////////////////////////////////////////

void UResourceBar::SetMaxValue(float newMaxValue, BarTransformationMode method) {
	//MaxValue = newMaxValue;
	//UE_LOG(LogTemp, Warning, TEXT("newMaxValue = %f"), newMaxValue);

	if (newMaxValue <= MinValue) {
		UE_LOG(LogTemp, Warning, TEXT("newMaxValue is not greater than MinValue! MaxValue was not changed."))
	}
	else {
		MaxValue = newMaxValue;
	}

	switch (method) {

	case BarTransformationMode::EXTEND: //THIS BEHAVIOUR MODE NEEDS TO BE BETTER DEFINED. RIGHT NOW IT IS NOT USED.
		//The pct stays the same, but the val changes
		CurrentValue = CurrentPct * MaxValue;
		//UE_LOG(LogTemp, Warning, TEXT("SCALE MaxValue to %f and CurrentPct is %f"), MaxValue, CurrentPct);
		//UE_LOG(LogTemp, Warning, TEXT("SCALE MaxValue to %f and CurrentPct [%f] = CurrentValue  [%f] / MaxValue [%f]"), MaxValue, CurrentPct, CurrentValue, MaxValue);
		break;
	case BarTransformationMode::SCALE:
		//The val stays the same, but the pct changes
		TargetPct = CurrentValue / MaxValue;
		//UE_LOG(LogTemp, Warning, TEXT("EXTEND MaxValue to %f and CurrentPct [%f] = CurrentValue  [%f] / MaxValue [%f]"), MaxValue, CurrentPct, CurrentValue, MaxValue);
		break;
	}



	UpdateText();
}

/*
* Just converts the arg to a percentage and passes it to SetPercent(float percent);
*/
void UResourceBar::SetValue(float value, float duration = 0.f) {
	SetPercent(value / MaxValue, duration);
}

void UResourceBar::SetPercent(float percent, float duration = 0.f) {

	//resets the animation timer
	RemainingAnimationTime = AnimationDuration;

	OriginalPct = CurrentPct;

	//set the target percentage to instantaneously update the interpolation rate
	TargetPct = FMath::Clamp(percent, 0.f, 1.f);

	//DISCRETE mode updates the text display upon every  input, and not continuously
	if (Mode == TextUpdateMode::DISCRETE) {
		UpdateText();
	}

	//insert the change request in the queue for dynamics and lag
	EventTime = GetWorld()->GetTimeSeconds();
	//HitQueue.Enqueue(ResourceEvent{ TargetPct, EventTime , duration });
}

float UResourceBar::GetMaxValue() {
	return MaxValue;
}

void UResourceBar::SetMainColor(FColor color) { MainColor = color; }

void UResourceBar::SetUpColor(FColor color) { UpColor = color; }

void UResourceBar::SetDownColor(FColor color) { DownColor = color; }

void UResourceBar::InitialRefresh(float health, float maxHealth) {
	SetValue(health);
	SetMaxValue(maxHealth, BarTransformationMode::SCALE);
}

////////////////////////////////////////////////////////////////
//  PRIVATE INTERFACE
////////////////////////////////////////////////////////////////

float UResourceBar::InterpolateProgress(UProgressBar* bar, float originalPct, float targetPct, float rem, float dur) {

	/*float NewPercent = UKismetMathLibrary::Ease(
		originalPct,
		targetPct,
		FMath::Clamp(1 - (rem / dur), 0.f, 1.f),
		EEasingFunc::SinusoidalOut);*/

		//float NewPercent = UKismetMathLibrary::Ease(
		//	originalPct,
		//	targetPct,
		//	FMath::Clamp(1 - (rem / dur), 0.f, 1.f),
		//	EEasingFunc::CircularOut);

	float NewPercent = UKismetMathLibrary::Ease(
		originalPct,
		targetPct,
		FMath::Clamp(1 - (rem / dur), 0.f, 1.f),
		EEasingFunc::EaseOut, 5.f);

	//UE_LOG(LogTemp, Warning, TEXT("rem: %4.4f, dur: %4.4f, val: %4.4f, og: %4.4f, target: %4.4f, NewPercent: %4.4f"), rem, dur, FMath::Clamp(1 - (rem / dur), 0.f, 1.f), originalPct, targetPct, NewPercent);

	bar->SetPercent(NewPercent);

	return NewPercent;
}

float UResourceBar::SetProgress(UProgressBar* bar, float targetPct) {
	float NewPercent = FMath::Clamp(targetPct, 0.f, 1.f);
	bar->SetPercent(NewPercent);
	return NewPercent;
}

void UResourceBar::UpdateText() {
	/*
if (DisplayMaxValue) {
	HealthTextBlock->SetText(FText::FromString(
		FString::FromInt((int)CurrentValue) + FString(" / ") + FString::FromInt((int)MaxValue)));// +FString("\n") +
		//FString::FromInt((int)SecondaryCurrentValue) + FString("/") + FString::FromInt((int)MaxValue)));
} else {
	HealthTextBlock->SetText(FText::FromString(
		FString::FromInt((int)CurrentValue)));// +FString("\n") +
		//FString::FromInt((int)SecondaryCurrentValue)));
}*/
}

