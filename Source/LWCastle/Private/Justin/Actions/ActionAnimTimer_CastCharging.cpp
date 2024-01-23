// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/ActionAnimTimer_CastCharging.h"

void UActionAnimTimer_CastCharging::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	StartTimer(this);
}

void UActionAnimTimer_CastCharging::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
}

void UActionAnimTimer_CastCharging::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
}

void UActionAnimTimer_CastCharging::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
}

void UActionAnimTimer_CastCharging::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
}

void UActionAnimTimer_CastCharging::ExecuteAction(AActor* InstigatorActor)
{
	Super::ExecuteAction(InstigatorActor);

	UE_LOG(LogTemp, Warning, TEXT("EXECUTING EACLKADJ ACTION!"));
	StartMontage(this);
	if (ensure(Cast<AActor>(GetOuter())))
	{
		OnActionInvoked.Broadcast(EActionType::START, Cast<AActor>(GetOuter()), GetTimerDuration());
	}
}

void UActionAnimTimer_CastCharging::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::OnNotifyBegin(NotifyName, BranchingPointPayload);
}
