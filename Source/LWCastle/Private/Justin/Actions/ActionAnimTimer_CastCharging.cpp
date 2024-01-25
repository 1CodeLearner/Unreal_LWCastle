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
	if (PlayerAttribute && PlayerAttribute->TryChannelMana(ManaChargingRate * DeltaTime))
	{
		AccumulatedMana += ManaChargingRate * DeltaTime;
		Widget->Update(CombatComp->GetActiveElementData().ChargeManaTotal, AccumulatedMana);
		if (AccumulatedMana >= CombatComp->GetActiveElementData().ChargeManaTotal)
		{
			ExecuteAction(GetGameplayComponent()->GetOwner());
		}
	}
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
	auto Attribute = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
	if (Attribute)
	{
		PlayerAttribute = Attribute;
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
	}
	Widget->SetVisibilityWidget(ESlateVisibility::Visible);
}

void UActionAnimTimer_CastCharging::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	if (IsMontagePlaying())
		StopMontage(this);
}

void UActionAnimTimer_CastCharging::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
	if (IsMontagePlaying())
		StopMontage(this);
	StartTick = false;
}

void UActionAnimTimer_CastCharging::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	StartMontage(this);

	StartTick = true;
}

void UActionAnimTimer_CastCharging::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);
	
	AccumulatedMana = 0.f;
	StartTick = false;
	PlayerAttribute->CancelChannelingMana();
	Widget->ResetWidget();
}

void UActionAnimTimer_CastCharging::ExecuteAction(AActor* InstigatorActor)
{
	Super::ExecuteAction(InstigatorActor);

	UE_LOG(LogTemp, Warning, TEXT("EXECUTING EACLKADJ ACTION!"));

	AccumulatedMana = 0.f;
	StartTick = false;
	PlayerAttribute->CompleteChannelingMana();
	GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);
	GetGameplayComponent()->StartActionByName(InstigatorActor, "ChargedState");
}

void UActionAnimTimer_CastCharging::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::OnNotifyBegin(NotifyName, BranchingPointPayload);

	UnbindNotifyEvent(this);
	StartTick = true;
}
