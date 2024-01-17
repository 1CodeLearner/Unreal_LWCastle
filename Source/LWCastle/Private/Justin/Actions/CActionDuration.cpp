// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionDuration.h"


void UCActionDuration::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (bHasDuration)
	{
		DurationDelegate.BindUFunction(this, "StopDuration", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, DurationDelegate, .1, false, ActionDuration);
	}
}

void UCActionDuration::StopAction_Implementation(AActor* InstigatorActor)
{
	if(bHasDuration && DurationHandle.IsValid())
		return;

	Super::StopAction_Implementation(InstigatorActor);
}

bool UCActionDuration::IsRunning() const
{
	return DurationHandle.IsValid();
}

void UCActionDuration::StopDuration(AActor* InstigatorActor)
{
	StopAction(InstigatorActor);
}

float UCActionDuration::GetActionDuration() const
{
	return ActionDuration;
}
