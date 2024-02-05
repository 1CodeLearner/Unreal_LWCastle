// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionAnimTimer_StunLight.h"
#include "Justin/AComponents/CGameplayComponent.h"

void UCActionAnimTimer_StunLight::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);
	bCanInterrupt = false;
}

void UCActionAnimTimer_StunLight::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (IsMontagePlaying()) 
	{
		StopMontage(this);
		StartMontage(this);
		return;
	}

	StartMontage(this);
	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UCActionAnimTimer_StunLight::OnMontageEnd);
	GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, Montage);

	StartTimer(this);
}

void UCActionAnimTimer_StunLight::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);
}

void UCActionAnimTimer_StunLight::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	StopMontage(this);
	ClearTimer();
}

bool UCActionAnimTimer_StunLight::CanStart_Implementation(AActor* InstigatorActor, UCAction* StartingAction) const
{
	return StartingAction == this || Super::CanStart_Implementation(InstigatorActor, StartingAction);
}

void UCActionAnimTimer_StunLight::OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted)
{
	if(!bInterrupted)
	{
		GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);
	}
}

void UCActionAnimTimer_StunLight::ExecuteAction(AActor* InstigatorActor)
{
	bCanInterrupt = true;
}
