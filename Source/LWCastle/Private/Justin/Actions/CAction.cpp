// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction.h"
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

	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->ActiveGameplayTags.RemoveTags(GetGrantedTags());
	}
	bIsRunning = false;
}

UCAction::UCAction()
{
	bIsRunning = false;
	bAutoStart = false;
}

void UCAction::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	if (GameplayComp)
	{
		this->GameplayCompRef = GameplayComp;
	}
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
	}
	bIsRunning = false;
}

bool UCAction::IsRunning() const
{
	return bIsRunning;
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