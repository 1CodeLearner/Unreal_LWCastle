// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionEffect.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/Actions/CAction.h"

void UCActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	//Making sure effect applied interval is not greater than effect duration.
	ensure(IntervalTime <= DurationTime);

	if (IntervalTime > DurationTime)
		return;

	if (DurationTime > 0.f)
	{
		DurationDelegate.BindUFunction(this, "DurationEnd", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, DurationDelegate, DurationTime, false);

		if (IntervalTime > 0.f)
		{
			IntervalDelegate.BindUFunction(this, "IntervalStart", InstigatorActor);
			GetWorld()->GetTimerManager().SetTimer(IntervalHandle, IntervalDelegate, IntervalTime, true);
		}
	}
	else
	{
		DurationEnd(InstigatorActor);
	}
}

void UCActionEffect::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	if (bDeleteAtEnd && ensure(GetGameplayComponent()))
	{
		GetGameplayComponent()->RemoveAction(this);
		ConditionalBeginDestroy();
	}
}

void UCActionEffect::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

}

void UCActionEffect::PauseAction_Implementation(AActor* InstigatorActor)
{
	//ensureAlways(IsRunning());
	Super::PauseAction_Implementation(InstigatorActor);
	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inside ActionEffect PauseAction"));

		Gameplay->PauseGameplayTags.AppendTags(GetGrantedTags());
		GetWorld()->GetTimerManager().PauseTimer(DurationHandle);
		GetWorld()->GetTimerManager().PauseTimer(IntervalHandle);
	}
}

void UCActionEffect::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	//ensureAlways(IsRunning());
	Super::UnPauseAction_Implementation(InstigatorActor);
	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inside ActionEffect UnPauseAction"));

		Gameplay->PauseGameplayTags.RemoveTags(GetGrantedTags());
		GetWorld()->GetTimerManager().UnPauseTimer(DurationHandle);
		GetWorld()->GetTimerManager().UnPauseTimer(IntervalHandle);
	}
}

void UCActionEffect::DurationEnd(AActor* InstigatorActor)
{
	if (IntervalHandle.IsValid() && GetWorld()->GetTimerManager().GetTimerRemaining(IntervalHandle) < KINDA_SMALL_NUMBER)
	{
		IntervalStart(InstigatorActor);
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);

	if (bDeleteAtEnd && ensure(GetGameplayComponent()))
	{
		GetGameplayComponent()->RemoveAction(this);
		ConditionalBeginDestroy();
	}

}

float UCActionEffect::GetDuration() const
{
	return DurationTime;
}

float UCActionEffect::GetIntervalTime() const
{
	return IntervalTime;
}

void UCActionEffect::IntervalStart(AActor* InstigatorActor)
{
	//Empty
}

UCActionEffect::UCActionEffect()
{
	bDeleteAtEnd = false;
}
