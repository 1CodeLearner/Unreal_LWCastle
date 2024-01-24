// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/ActionAnimTimer_CastCharging.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/CPlayerController.h"

void UActionAnimTimer_CastCharging::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Working here Calling tick from %s"),
		*GetNameSafe(this));
}

TStatId UActionAnimTimer_CastCharging::GetStatId() const
{
	TStatId Stat;
	return Stat;
}

bool UActionAnimTimer_CastCharging::IsTickable() const
{
	return true;
}

bool UActionAnimTimer_CastCharging::IsAllowedToTick() const
{
	return StartTick;
}

void UActionAnimTimer_CastCharging::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	auto PlayerController = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) {
		Widget = PlayerController->ChargeWidget;
	}
}

void UActionAnimTimer_CastCharging::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	if (ensure(!IsTimerValid()))
	{
		StartMontage(this);
		
		StartTimer(this);

		//OnActionInvoked.Broadcast(EActionType::START, InstigatorActor, GetTimerDuration());
	}
	UE_LOG(LogTemp, Warning, TEXT("Show duration: %f"), GetTimerDuration());
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
	//StartMontage(this);
	//if (ensure(Cast<AActor>(GetOuter())))
	//{
	//	OnActionInvoked.Broadcast(EActionType::START, Cast<AActor>(GetOuter()), GetTimerDuration());
	//}

	StartTick = true;
}

void UActionAnimTimer_CastCharging::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::OnNotifyBegin(NotifyName, BranchingPointPayload);
}
