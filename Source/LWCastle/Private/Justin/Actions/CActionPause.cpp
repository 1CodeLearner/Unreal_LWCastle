// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionPause.h"

bool UCActionPause::CanPause(AActor* InstigatorActor, UCAction* OtherAction) const
{
	if (bCanPause)
	{
		if (PausedTags.HasAny(OtherAction->GetGrantedTags()))
		{
			return !bIsPausing && IsRunning();
		}
	}

	return false;
}

void UCActionPause::PauseAction_Implementation(AActor* InstigatorActor)
{
	bIsPausing = true;

}

bool UCActionPause::CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const
{
	if (bCanPause)
	{
		if (PausedTags.HasAny(OtherAction->GetGrantedTags()))
		{
			return bIsPausing && IsRunning();
		}
	}

	return false;
}

void UCActionPause::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	bIsPausing = false;
}

bool UCActionPause::IsPausing() const
{
	return bIsPausing;
}

void UCActionPause::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	ensure(!bIsPausing);

	bIsPausing = false;
}

void UCActionPause::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);

	if (IsPausing())
	{
		Execute_UnPauseAction(this, InstigatorActor);
	}
}

UCActionPause::UCActionPause()
{
	bIsPausing = false;
	bCanPause = false;
}
