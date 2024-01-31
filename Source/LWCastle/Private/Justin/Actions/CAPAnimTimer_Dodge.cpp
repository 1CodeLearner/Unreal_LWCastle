// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAPAnimTimer_Dodge.h"
#include "Justin/AComponents/CGameplayComponent.h"

void UCAPAnimTimer_Dodge::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);


}

void UCAPAnimTimer_Dodge::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (IsMontagePlaying())
		return;

	StartMontage(this);
	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UCAPAnimTimer_Dodge::OnMontageEnd);
	GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, Montage);
}

void UCAPAnimTimer_Dodge::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);
}

void UCAPAnimTimer_Dodge::OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted)
{
	GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);
	StopMontage(this);
}
