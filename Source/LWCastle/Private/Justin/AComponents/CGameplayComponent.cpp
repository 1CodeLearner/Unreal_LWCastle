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

void UCGameplayComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (auto Action : Actions)
	{
		if (Action && Action->GetActionName() == ActionName)
		{
			if (Action->CanStart(InstigatorActor))
			{
				Action->StartAction(InstigatorActor);
				break;
			}
		}
	}
}

void UCGameplayComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (auto Action : Actions)
	{
		if (Action && Action->GetActionName() == ActionName)
		{
			if(Action->IsRunning())
			{
				Action->StopAction(InstigatorActor);
				break;
			}
		}
	}
}

UCGameplayComponent::UCGameplayComponent()
{

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
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Black, DebugMsg);

	//Draw All Actions
	/*for (auto* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}*/
}


TArray<UCAction*> UCGameplayComponent::GetActions() const
{
	return Actions;
}
