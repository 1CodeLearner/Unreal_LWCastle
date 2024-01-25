// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionPause.h"

#include "Justin/AComponents/CGameplayComponent.h"

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
	GetGameplayComponent()->PauseGameplayTags.AppendTags(GetGrantedTags());
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
	GetGameplayComponent()->PauseGameplayTags.RemoveTags(GetGrantedTags());
	bIsPausing = false;
}

bool UCActionPause::IsPausing() const
{
	return bIsPausing;
}

void UCActionPause::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	//Commenting so it doesn't trigger when pressing Sprint while ActionPause_Sprint and ActionEffect_Jump are both active
	//ensure(!bIsPausing);

	if (GetGameplayComponent()->PauseGameplayTags.HasAnyExact(GetGrantedTags()))
	{
		GetGameplayComponent()->PauseGameplayTags.RemoveTags(GetGrantedTags());
	}

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
