// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction.h"

#include "AssetTypeCategories.h"
#include "Justin/AComponents/CGameplayComponent.h"


bool UCAction::CanStart_Implementation(AActor* InstigatorActor) const
{
	if (IsRunning())
		return false;

	auto GameplayComp = GetGameplayComponent();
	if (GameplayComp && GameplayComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void UCAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running StartAction %s"), *this->GetName());

	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->ActiveGameplayTags.AppendTags(GetGrantedTags());
	}

	bIsRunning = true;
}

void UCAction::CompleteAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running CompleteAction %s"), *this->GetName());

	ensure(!bIsPausing);

	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->ActiveGameplayTags.RemoveTags(GetGrantedTags());
		Gameplay->PauseGameplayTags.RemoveTags(GetGrantedTags());
	}
	bIsRunning = false;
	bIsPausing = false;
}

UCAction::UCAction()
{
	bIsRunning = false;
	bIsPausing = false;
	bAutoStart = false;
}

void UCAction::Initialize(UCGameplayComponent* GameplayComp)
{
	if (GameplayComp)
	{
		this->GameplayCompRef = GameplayComp;
	}
}

bool UCAction::CanPause(AActor* InstigatorActor, UCAction* OtherAction)
{
	if (bCanPause)
	{
		if (PausedTags.HasAny(OtherAction->GetGrantedTags()) && 
			!GetGameplayComponent()->PauseGameplayTags.HasAllExact(GetGrantedTags()))
		{
			return !bIsPausing && bIsRunning;
		}
	}

	return false;
}

void UCAction::PauseAction_Implementation(AActor* InstigatorActor)
{
	ensure(IsRunning());
	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->PauseGameplayTags.AppendTags(GetGrantedTags());
	}
}

bool UCAction::CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const
{
	if (bCanPause)
	{
		if ( PausedTags.HasAny(OtherAction->GetGrantedTags()) && 
			GetGameplayComponent()->PauseGameplayTags.HasAllExact(GetGrantedTags()) )
		{
			return bIsPausing && bIsRunning;
		}
	}

	return false;
}

void UCAction::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->PauseGameplayTags.RemoveTags(GetGrantedTags());
	}
	
	bIsPausing = false;
}

bool UCAction::CanInterrupt(AActor* InstigatorActor, UCAction* OtherAction) const
{
	if (bCanInterrupt)
	{
		if (IsRunning() && InterruptedTags.HasAny(OtherAction->GetGrantedTags()))
		{
			return true;
		}
		return false;
	}
	return false;
}

void UCAction::InterruptAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running InterruptAction %s"), *this->GetName());

	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->ActiveGameplayTags.RemoveTags(GetGrantedTags());
		Gameplay->PauseGameplayTags.RemoveTags(GetGrantedTags());
	}
	bIsRunning = false;
	bIsPausing = false;
}

bool UCAction::IsRunning() const
{
	return bIsRunning;
}

bool UCAction::IsPausing() const
{
	return bIsPausing;
}

UWorld* UCAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

FName UCAction::GetActionName() const
{
	return ActionName;
}

bool UCAction::IsAutoStart() const
{
	return bAutoStart;
}


FGameplayTagContainer UCAction::GetGrantedTags() const
{
	return GrantedTags;
}

UCGameplayComponent* UCAction::GetGameplayComponent() const
{
	return GameplayCompRef;
}