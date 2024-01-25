// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionEffect_ChargedState.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/CPlayerController.h"
#include "Justin/Widgets/CChargeWidget.h"

UCActionEffect_ChargedState::UCActionEffect_ChargedState()
{

}

void UCActionEffect_ChargedState::Tick(float DeltaTime)
{
	Widget->Update(DurationTime, GetWorld()->GetTimerManager().GetTimerRemaining(DurationHandle));
}

TStatId UCActionEffect_ChargedState::GetStatId() const
{
	TStatId Stat;
	return Stat;
}

bool UCActionEffect_ChargedState::IsTickable() const
{
	return true;
}

bool UCActionEffect_ChargedState::IsAllowedToTick() const
{
	return StartTick;
}


void UCActionEffect_ChargedState::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	auto PlayerController = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) {
		Widget = PlayerController->ChargeWidget;
	}
}


void UCActionEffect_ChargedState::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	StartTick = true;
}

void UCActionEffect_ChargedState::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	StartTick = false;
	Widget->ResetWidget();
}

void UCActionEffect_ChargedState::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StartTick = false;
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	Widget->ResetWidget();
}