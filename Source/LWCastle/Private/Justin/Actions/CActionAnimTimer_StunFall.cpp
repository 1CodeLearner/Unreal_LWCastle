// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionAnimTimer_StunFall.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/Actions/CAPAnimTimer_Dodge.h"

void UCActionAnimTimer_StunFall::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);
	DodgeAction = Cast<UCAPAnimTimer_Dodge>(GetGameplayComponent()->GetActionByName("Roll"));
	ensure(DodgeAction);
}

void UCActionAnimTimer_StunFall::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (IsMontagePlaying()) 
	{
		StopMontage(this);
		ClearTimer();
	}


	StartMontage(this);

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UCActionAnimTimer_StunFall::OnMontageEnd);
	GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, Montage);

	DodgeAction->AddBlockedTag(GetGrantedTags());
	StartTimer(this);
}

void UCActionAnimTimer_StunFall::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);
	ClearTimer();
	GetGameplayComponent()->RemoveAction(this);
}

void UCActionAnimTimer_StunFall::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	StopMontage(this);
	DodgeAction->RemoveBlockedTag(GetGrantedTags());
	ClearTimer();
	GetGameplayComponent()->RemoveAction(this);
}

void UCActionAnimTimer_StunFall::OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted)
{
	if(!bInterrupted)
	{
		GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);
	}
}

void UCActionAnimTimer_StunFall::ExecuteAction(AActor* InstigatorActor)
{
	DodgeAction->RemoveBlockedTag(GetGrantedTags());
}
