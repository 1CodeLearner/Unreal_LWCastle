// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionEffect.h"
#include "Justin/AComponents/CGameplayComponent.h"

void UCActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	//Making sure effect applied interval is not greater than effect duration.
	ensure(IntervalTime <= DurationTime);

	if(IntervalTime > DurationTime)
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

void UCActionEffect::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
	
	UE_LOG(LogTemp, Warning, TEXT("Inside ActionEffect PauseAction"));
}

void UCActionEffect::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	
	UE_LOG(LogTemp, Warning, TEXT("Inside ActionEffect UnPauseAction"));

}


void UCActionEffect::DurationEnd(AActor* InstigatorActor)
{
	if (IntervalHandle.IsValid() && GetWorld()->GetTimerManager().GetTimerRemaining(IntervalHandle) < KINDA_SMALL_NUMBER)
	{
		IntervalStart(InstigatorActor);
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if(ensure(GetGameplayComponent()))
	{
		GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);
		GetGameplayComponent()->RemoveAction(this);
	}
}

void UCActionEffect::IntervalStart(AActor* InstigatorActor)
{
	//Empty
}
