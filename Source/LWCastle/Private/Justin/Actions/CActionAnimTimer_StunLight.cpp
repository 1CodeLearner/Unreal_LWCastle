// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionAnimTimer_StunLight.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Uwol/uwol_test.h"

void UCActionAnimTimer_StunLight::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (IsMontagePlaying())
	{
		UnbindDelegate();
		StopMontage(this);
		//ClearTimer();
	}

	StartMontage(this);
	if (ensureAlways(!GetAnimInstance()->Montage_GetBlendingOutDelegate()->IsBound()))
	{
		MontageEndDelegate.BindUObject(this, &UCActionAnimTimer_StunLight::OnMontageEnd);
		GetAnimInstance()->Montage_SetBlendingOutDelegate(MontageEndDelegate, Montage);
	}

	//StartTimer(this);
}

void UCActionAnimTimer_StunLight::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);
	//ClearTimer();
}

void UCActionAnimTimer_StunLight::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	StopMontage(this);
	//ClearTimer();
}

bool UCActionAnimTimer_StunLight::CanStart_Implementation(AActor* InstigatorActor, UCAction* StartingAction) const
{
	return StartingAction == this || Super::CanStart_Implementation(InstigatorActor, StartingAction);
}

void UCActionAnimTimer_StunLight::OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted)
{
	GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);

	UnbindDelegate();
}

/*void UCActionAnimTimer_StunLight::ExecuteAction(AActor* InstigatorActor)
{
	auto Player = Cast<Auwol_test>(InstigatorActor);
	if(Player)
	{
		Player->bIsStunned = false;
	}
}*/

void UCActionAnimTimer_StunLight::UnbindDelegate()
{
	auto Delegate = GetAnimInstance()->Montage_GetBlendingOutDelegate();

	if (Delegate && Delegate->IsBound())
	{
		GetAnimInstance()->Montage_GetBlendingOutDelegate()->Unbind();
	}
}