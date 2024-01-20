// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/Actions/CAction.h"
#include "Justin/Actions/CAction_MagicAttack.h"

void UCGameplayComponent::AddAction(TSubclassOf<UCAction> NewActionClass)
{
	if (NewActionClass)
	{
		UCAction* NewAction = NewObject<UCAction>(GetOwner(), NewActionClass);
		if (NewAction)
		{
			NewAction->Initialize(this);
			Actions.Add(NewAction);
		}
	}
}

void UCGameplayComponent::RemoveAction(UCAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		Actions.Remove(ActionToRemove);
	}
}

void UCGameplayComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	if (ensure(!ActionName.IsNone() && InstigatorActor))
	{
		UCAction* ActionFound = nullptr;
		for (auto Action : Actions)
		{
			if (Action && Action->GetActionName() == ActionName)
			{
				if (Action->CanStart(InstigatorActor))
				{
					Action->StartAction(InstigatorActor);

					UE_LOG(LogTemp, Warning, TEXT("StartAction: %s"), *Action->GetActionName().ToString());
					ActionFound = Action;
					break;
				}
			}
		}
		if (ActionFound)
		{
			for (auto Action : Actions)
			{
				if (Action != ActionFound)
				{
					if (Action->CanInterrupt(InstigatorActor, ActionFound))
					{
						Action->InterruptAction(InstigatorActor);
						UE_LOG(LogTemp, Warning, TEXT("InterruptAction: %s"), *Action->GetActionName().ToString());
					}

					if (Action->CanPause(InstigatorActor, ActionFound))
					{
						Action->PauseAction(InstigatorActor);
						UE_LOG(LogTemp, Warning, TEXT("PauseAction: %s"), *Action->GetActionName().ToString());
					}
				}
			}
		}
	}
}

void UCGameplayComponent::CompleteActionByName(AActor* InstigatorActor, FName ActionName)
{
	if (ensure(!ActionName.IsNone() && InstigatorActor))
	{
		UCAction* ActionFound = nullptr;
		for (auto Action : Actions)
		{
			if (Action && Action->GetActionName() == ActionName)
			{
				if (Action->IsRunning() && !Action->IsPausing())
				{
					Action->CompleteAction(InstigatorActor);
					ActionFound = Action;
					UE_LOG(LogTemp, Warning, TEXT("Inside CompleteAction"));
					break;
				}
			}
		}
		if (ActionFound)
		{
			for (auto Action : Actions)
			{
				if (Action != ActionFound)
				{
					if (Action->CanUnPause(InstigatorActor, Action))
					{
						Action->UnPauseAction(InstigatorActor);
					}
				}
			}
		}
	}
}

void UCGameplayComponent::CompleteActionBy(AActor* InstigatorActor, UCAction* ActionToComplete)
{
	if (ensure(ActionToComplete && InstigatorActor))
	{
		bool HasCompleted = false;

		if (Actions.Contains(ActionToComplete))
		{
			if (ActionToComplete->IsRunning())
			{
				ActionToComplete->CompleteAction(InstigatorActor);
				HasCompleted = true;
				UE_LOG(LogTemp, Warning, TEXT("Inside CompleteAction"));
			}
		}

		if (HasCompleted)
		{
			for (auto Action : Actions)
			{
				if (Action != ActionToComplete)
				{
					if (Action->CanUnPause(InstigatorActor, Action))
					{
						Action->UnPauseAction(InstigatorActor);
					}
				}
			}
		}
	}
}

UCGameplayComponent::UCGameplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UCGameplayComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto ActionClass : ActionClasses)
	{
		AddAction(ActionClass);
	}
}

void UCGameplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, DebugMsg);

	//Draw All Actions
	for (auto* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Black, DebugMsg);

		//LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

TArray<UCAction*> UCGameplayComponent::GetActions() const
{
	return Actions;
}
