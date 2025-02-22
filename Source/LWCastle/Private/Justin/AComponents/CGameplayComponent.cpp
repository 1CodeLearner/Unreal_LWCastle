// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CGameplayComponent.h"

#include "Justin/CActionPauseInterface.h"
#include "Justin/Actions/CAction.h"

void UCGameplayComponent::AddAction(AActor* InstigatorActor, TSubclassOf<UCAction> NewActionClass)
{
	if (ensure(InstigatorActor && NewActionClass))
	{
		bool bHasDuplicate = false;
		for (auto Action : Actions)
		{
			auto value1 = NewActionClass->GetDefaultObject();
			auto value2 = Action->GetClass();
			if (Action->IsA(NewActionClass))
			{
				bHasDuplicate = true;
				break;
			}
		}

		if (NewActionClass && !bHasDuplicate)
		{
			UCAction* NewAction = NewObject<UCAction>(GetOwner(), NewActionClass);
			if (NewAction)
			{
				NewAction->Initialize(this);
				Actions.Add(NewAction);

				if (NewAction->IsAutoStart())
				{
					StartActionBy(InstigatorActor, NewAction);
				}
			}

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

UCAction* UCGameplayComponent::GetActionByName(FName ActionName) const
{
	if (ensureAlways(!ActionName.IsNone()))
	{
		bool ActionFound = true;
		for (auto Action : Actions)
		{
			if (Action->GetActionName() == ActionName)
				return Action;
		}
		ensureAlwaysMsgf(ActionFound, TEXT("Action &s could not be found"), *ActionName.ToString());
	}

	return nullptr;
}

void UCGameplayComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	UCAction* ActionStart = nullptr;

	if (ensure(!ActionName.IsNone() && InstigatorActor))
	{
		for (auto Action : Actions)
		{
			if (Action && Action->GetActionName() == ActionName)
			{
				if (Action->CanStart(InstigatorActor, Action))
				{
					ActionStart = Action;
					break;
				}
			}
		}
		if (ActionStart)
		{
			TArray<UCAction*> ActionsToInterrupt;
			for (auto Action : Actions)
			{
				UCAction* ActionTemp = ProcessInterruptAndPause(InstigatorActor, Action, ActionStart);
				if (ActionTemp)
					ActionsToInterrupt.Add(ActionTemp);
			}

			if (!ActionsToInterrupt.IsEmpty())
			{
				for (auto Action : ActionsToInterrupt)
				{
					Action->InterruptAction(InstigatorActor);
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("StartAction: %s"), *ActionStart->GetActionName().ToString());
			ActionStart->StartAction(InstigatorActor);
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
				if (Action->IsRunning())
				{
					ActionFound = Action;
					break;
				}
			}
		}
		if (ActionFound)
		{
			for (auto Action : Actions)
			{
				ProcessUnPause(InstigatorActor, Action, ActionFound);
			}
			ActionFound->CompleteAction(InstigatorActor);
		}
	}
}

void UCGameplayComponent::StartActionBy(AActor* InstigatorActor, UCAction* ActionToStart)
{
	if (ensure(ActionToStart && InstigatorActor))
	{
		bool HasStarted = false;

		if (Actions.Contains(ActionToStart))
		{
			if (ActionToStart->CanStart(InstigatorActor, ActionToStart))
			{
				ActionToStart->StartAction(InstigatorActor);

				UE_LOG(LogTemp, Warning, TEXT("StartAction: %s"), *ActionToStart->GetActionName().ToString());
				HasStarted = true;
			}
		}

		if (HasStarted)
		{
			TArray<UCAction*> ActionsToInterrupt;
			for (auto Action : Actions)
			{
				UCAction* ActionTemp = ProcessInterruptAndPause(InstigatorActor, Action, ActionToStart);
				if (ActionTemp)
					ActionsToInterrupt.Add(ActionTemp);

			}

			if (!ActionsToInterrupt.IsEmpty())
			{
				for (auto Action : ActionsToInterrupt)
				{
					Action->InterruptAction(InstigatorActor);

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
			}
		}

		if (HasCompleted)
		{
			for (auto Action : Actions)
			{
				ProcessUnPause(InstigatorActor, Action, ActionToComplete);
			}
		}
	}
}

void UCGameplayComponent::StopAllActions(AActor* InstigatorActor)
{
	for (auto Action : Actions)
	{
		if (IsValid(Action))
			Action->CompleteAction(InstigatorActor);
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
		AddAction(GetOwner(), ActionClass);
	}
}

void UCGameplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString ActiveDebugMsg = GetNameSafe(GetOwner()) + " Active : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, ActiveDebugMsg);

	FString PauseDebugMsg = GetNameSafe(GetOwner()) + " Paused : " + PauseGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, PauseDebugMsg);


	//Draw All Actions
	//for (auto* Action : Actions)
	//{
	//	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	//	FString ActionMsg = FString::Printf(TEXT("[%s] Actions Owned: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	//	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Magenta, ActionMsg);

	//	//LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	//}
}


UCAction* UCGameplayComponent::ProcessInterruptAndPause(AActor* Instigator, UCAction* ActionProcessed, UCAction* ActionCompared)
{
	if (ActionProcessed != ActionCompared)
	{
		if (ActionProcessed->CanInterrupt(Instigator, ActionCompared))
		{
			ProcessUnPause(Instigator, ActionProcessed, ActionCompared);
			UE_LOG(LogTemp, Warning, TEXT("InterruptAction: %s"), *ActionProcessed->GetActionName().ToString());
			return ActionProcessed;
		}
		else
		{
			if (ActionProcessed->Implements<UCActionPauseInterface>())
			{
				auto PauseInterface = Cast<ICActionPauseInterface>(ActionProcessed);
				if (PauseInterface->CanPause(Instigator, ActionCompared))
				{
					PauseInterface->Execute_PauseAction(ActionProcessed, Instigator);
					UE_LOG(LogTemp, Warning, TEXT("PauseAction: %s"), *ActionProcessed->GetActionName().ToString());
				}
			}
		}
	}
	return nullptr;
}

void UCGameplayComponent::ProcessUnPause(AActor* Instigator, UCAction* ActionProcessed, UCAction* ActionCompared)
{
	if (ActionProcessed->Implements<UCActionPauseInterface>() && ActionProcessed != ActionCompared)
	{
		auto PauseInterface = Cast<ICActionPauseInterface>(ActionProcessed);
		if (PauseInterface->CanUnPause(Instigator, ActionCompared))
		{
			PauseInterface->Execute_UnPauseAction(ActionProcessed, Instigator);
			UE_LOG(LogTemp, Warning, TEXT("UnPauseAction: %s"), *ActionProcessed->GetActionName().ToString());
		}
	}
}

TArray<UCAction*> UCGameplayComponent::GetActions() const
{
	return Actions;
}