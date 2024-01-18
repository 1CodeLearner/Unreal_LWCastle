// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicMulti.h"

void UCMagicMulti::Press_Implementation(AActor* InstigatorActor)
{
	Super::Press_Implementation(InstigatorActor);

	MagicDelegate.BindUFunction(this, "MagicExecute", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(MagicHandle, MagicDelegate, GetDelayTime(), true);
}

void UCMagicMulti::Release_Implementation(AActor* InstigatorActor)
{
	Super::Release_Implementation(InstigatorActor);
	if (MagicHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(MagicHandle);
}

void UCMagicMulti::Reset_Implementation(AActor* InstigatorActor)
{
	Super::Reset_Implementation(InstigatorActor);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}