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
	bIsPausing = false;
}

void UCActionEffect::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	ensure(!bIsPausing);

	bIsPausing = false;
}

bool UCActionEffect::CanPause(AActor* InstigatorActor, UCAction* OtherAction) const
{
	if (bCanPause)
	{
		if (PausedTags.HasAny(OtherAction->GetGrantedTags()) &&
			!GetGameplayComponent()->PauseGameplayTags.HasAllExact(GetGrantedTags()))
		{
			return !bIsPausing && OtherAction->IsRunning();
		}
	}

	return false;
}

void UCActionEffect::PauseAction_Implementation(AActor* InstigatorActor)
{
	ensure(IsRunning());
	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->PauseGameplayTags.AppendTags(GetGrantedTags());
		GetWorld()->GetTimerManager().PauseTimer(DurationHandle);
		GetWorld()->GetTimerManager().PauseTimer(IntervalHandle);
	}
	
	bIsPausing = true;
}


bool UCActionEffect::CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const
{
	if (bCanPause)
	{
		if (PausedTags.HasAny(OtherAction->GetGrantedTags()) &&
			GetGameplayComponent()->PauseGameplayTags.HasAllExact(GetGrantedTags()))
		{
			return bIsPausing && OtherAction->IsRunning();
		}
	}

	return false;
}

void UCActionEffect::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Inside ActionEffect UnPauseAction"));
	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->PauseGameplayTags.RemoveTags(GetGrantedTags());
		GetWorld()->GetTimerManager().PauseTimer(DurationHandle);
		GetWorld()->GetTimerManager().PauseTimer(IntervalHandle);
	}

	bIsPausing = false;
}

bool UCActionEffect::IsPausing() const
{
	return bIsPausing;
}


void UCActionEffect::DurationEnd(AActor* InstigatorActor)
{
	if (IntervalHandle.IsValid() && GetWorld()->GetTimerManager().GetTimerRemaining(IntervalHandle) < KINDA_SMALL_NUMBER)
	{
		IntervalStart(InstigatorActor);
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (ensure(GetGameplayComponent()))
	{
		GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);
		GetGameplayComponent()->RemoveAction(this);
	}
}

void UCActionEffect::IntervalStart(AActor* InstigatorActor)
{
	//Empty
}

UCActionEffect::UCActionEffect()
{
	bIsPausing = false;
	bCanPause = false;
}
