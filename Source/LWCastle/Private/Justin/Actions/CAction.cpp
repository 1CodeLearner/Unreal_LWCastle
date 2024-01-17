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
		Gameplay->ActiveGameplayTags.AppendTags(GrantedTags);
	}


	if (bHasDuration)
	{
		DurationDelegate.BindUFunction(this, "StopDuration", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, DurationDelegate, .1, false, ActionDuration);
	}


	bIsRunning = true;
}

void UCAction::InterruptAction_Implementation(AActor* InstigatorActor)
{
	StopAction(InstigatorActor);
}

void UCAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running StopAction %s"), *this->GetName());

	auto Gameplay = GetGameplayComponent();
	if (Gameplay)
	{
		Gameplay->ActiveGameplayTags.RemoveTags(GrantedTags);
	}
	bIsRunning = false;
}


bool UCAction::CanInterrupt_Implementation(AActor* InstigatorActor, FGameplayTagContainer OtherGrantedTag) const
{
	if (bCanInterrupt)
	{
		if (InterruptedTags.HasAny(OtherGrantedTag))
		{
			return true;
		}
		return false;
	}
	return false;
}

void UCAction::StopDuration(AActor* InstigatorActor)
{
	StopAction(InstigatorActor);
}

UCAction::UCAction()
{
	bIsRunning = false;
}

void UCAction::Initialize(UCGameplayComponent* GameplayComp)
{
	if (GameplayComp)
	{
		this->GameplayCompRef = GameplayComp;
	}
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

UCGameplayComponent* UCAction::GetGameplayComponent() const
{
	return GameplayCompRef;
}

float UCAction::GetActionDuration() const
{
	return ActionDuration;
}

