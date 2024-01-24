// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/ActionAnimTimer_CastCharging.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/CPlayerController.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CCombatComponent.h"

UActionAnimTimer_CastCharging::UActionAnimTimer_CastCharging()
{
	ManaChargingRate = 1.0f;
	AccumulatedMana = 0.f;
}

void UActionAnimTimer_CastCharging::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Working here Calling tick from %s"),
	//	*GetNameSafe(this));
	UE_LOG(LogTemp, Warning, TEXT("Show duration: %f"), GetTimerDuration());
	UE_LOG(LogTemp, Warning, TEXT("Show Remaining: %f"), GetTimerRemaining());
	UE_LOG(LogTemp, Warning, TEXT("Is Animation Playing? %s"), (IsMontagePlaying() ? TEXT("TRUE"): TEXT("False")));
	//AccumulatedMana += ManaChargingRate * DeltaTime;
	//Widget->Update(AccumulatedMana,
	//	CombatComp->GetActiveElementData().ChargeManaTotal);
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
	auto PlayerTemp = Cast<UCPlayerAttributeComp>(GetGameplayComponent()->GetOwner());
	if (PlayerTemp) 
	{
		PlayerAttribute = PlayerTemp;
	}
	auto CombatCompTemp = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCCombatComponent>();
	if (CombatCompTemp) 
	{
		CombatComp = CombatCompTemp;
	}
}

void UActionAnimTimer_CastCharging::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	if (ensure(!IsTimerValid() && !IsMontagePlaying()))
	{
		StartMontage(this);
		//OnActionInvoked.Broadcast(EActionType::START, InstigatorActor, GetTimerDuration());
	}
	Widget->SetVisibilityWidget(ESlateVisibility::Visible);
}

void UActionAnimTimer_CastCharging::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	if(IsMontagePlaying())
		StopMontage(this);
	
	ClearTimer();
}

void UActionAnimTimer_CastCharging::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
	if (IsMontagePlaying())
		StopMontage(this);
	if (IsTimerValid())
		PauseTimer();
}

void UActionAnimTimer_CastCharging::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	StartMontage(this);
}

void UActionAnimTimer_CastCharging::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	ClearTimer();
	StopMontage(this);
	StartTick = false;
	Widget->ResetWidget();
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

	StartTick = false;
	GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);
	GetGameplayComponent()->StartActionByName(InstigatorActor, "ChargedState");
}

void UActionAnimTimer_CastCharging::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::OnNotifyBegin(NotifyName, BranchingPointPayload);

	UnbindNotifyEvent(this);
	if (IsTimerValid())
		UnPauseTimer();
	else
		StartTimer(this);
	StartTick = true;
}
