// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction.h"

#include "AssetTypeCategories.h"
#include "Justin/AComponents/CGameplayComponent.h"

void UCAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running StartAction %s"), *this->GetName());

	auto Gameplay = GetOwnerComponent();
	Gameplay->ActiveGameplayTags.AppendTags(GrantedTags);
	bIsRunning = true;
}

void UCAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running StopAction %s"), *this->GetName());

	auto Gameplay = GetOwnerComponent();
	Gameplay->ActiveGameplayTags.RemoveTags(GrantedTags);
	bIsRunning = false;
}

UCAction::UCAction()
{
	bIsRunning = false;
}

bool UCAction::CanStart_Implementation(AActor* InstigatorActor) const
{
	if (IsRunning())
		return false;

	auto GameplayComp = GetOwnerComponent();
	if (GameplayComp && GameplayComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

bool UCAction::CanStop_Implementation(AActor* InstigatorActor, FGameplayTagContainer OtherGrantedTag) const
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

bool UCAction::IsRunning() const
{
	return bIsRunning;
}

UWorld* UCAction::GetWorld() const
{
	UCGameplayComponent* GameplayComp = Cast<UCGameplayComponent>(GetOuter());
	if (GameplayComp)
	{
		return GameplayComp->GetWorld();
	}
	return nullptr;
}

FName UCAction::GetActionName() const
{
	return ActionName;
}

UCGameplayComponent* UCAction::GetOwnerComponent() const
{
	return Cast<UCGameplayComponent>(GetOuter());
}

