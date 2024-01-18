// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicSingle.h"

void UCMagicSingle::Press_Implementation(AActor* InstigatorActor)
{
	Super::Press_Implementation(InstigatorActor);

	if(MagicHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(MagicHandle);

	MagicDelegate.BindUFunction(this, "MagicExecute", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(MagicHandle, MagicDelegate, .005f, false);
	GetWorld()->GetTimerManager().PauseTimer(MagicHandle);
}

void UCMagicSingle::Release_Implementation(AActor* InstigatorActor)
{
	Super::Release_Implementation(InstigatorActor);

	if (bIsCoolDown) return;

	if(MagicHandle.IsValid())
		GetWorld()->GetTimerManager().UnPauseTimer(MagicHandle);
}

void UCMagicSingle::Reset_Implementation(AActor* InstigatorActor)
{
	Super::Reset_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UCMagicSingle::MagicExecute(AActor* InstigatorActor)
{
	Super::MagicExecute(InstigatorActor);
	if (ensure(InstigatorActor))
	{
		GetWorld()->GetTimerManager().ClearTimer(MagicHandle);
		CooldownDelegate.BindUFunction(this, "StopCooldown", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(CooldownHandle, CooldownDelegate, 0.005f, false, GetDelayTime());
		bIsCoolDown = true;
	}
}

void UCMagicSingle::StopCooldown(AActor* InstigatorActor)
{
	bIsCoolDown = false;
}

UCMagicSingle::UCMagicSingle()
{
	bIsCoolDown = false;
}
