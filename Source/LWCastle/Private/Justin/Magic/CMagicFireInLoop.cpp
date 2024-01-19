// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicFireInLoop.h"

void UCMagicFireInLoop::Press_Implementation(AActor* InstigatorActor)
{
	Super::Press_Implementation(InstigatorActor);
	
	StartMontage();
	/*MagicDelegate.BindUFunction(this, "MagicExecute", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(MagicHandle, MagicDelegate, GetDelayTime(), true);*/
}

void UCMagicFireInLoop::Release_Implementation(AActor* InstigatorActor)
{
	Super::Release_Implementation(InstigatorActor);
	StopMontage();
	/*if (MagicHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(MagicHandle);*/
}

void UCMagicFireInLoop::MagicExecute(AActor* InstigatorActor)
{
	Super::MagicExecute(InstigatorActor);
}
