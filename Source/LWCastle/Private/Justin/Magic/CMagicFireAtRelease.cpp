// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicFireAtRelease.h"

void UCMagicFireAtRelease::Press_Implementation(AActor* InstigatorActor)
{
	Super::Press_Implementation(InstigatorActor);

	/*if(MagicHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(MagicHandle);*/

		//MagicDelegate.BindUFunction(this, "MagicExecute", InstigatorActor);
		//GetWorld()->GetTimerManager().SetTimer(MagicHandle, MagicDelegate, .005f, false);
		//GetWorld()->GetTimerManager().PauseTimer(MagicHandle);
}

void UCMagicFireAtRelease::Release_Implementation(AActor* InstigatorActor)
{
	Super::Release_Implementation(InstigatorActor);

	if (bIsCoolDown) return;

	StartMontage();
	//GetWorld()->GetTimerManager().UnPauseTimer(MagicHandle);
}

void UCMagicFireAtRelease::Reset_Implementation(AActor* InstigatorActor)
{
	Super::Reset_Implementation(InstigatorActor);
	bIsCoolDown = false;
}

void UCMagicFireAtRelease::MagicExecute(AActor* InstigatorActor)
{
	Super::MagicExecute(InstigatorActor);
	UE_LOG(LogTemp, Warning, TEXT("MagicExecute in MagicFireAtRelease"));
}

void UCMagicFireAtRelease::StartMontage()
{
	Super::StartMontage();
	bIsCoolDown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &UCMagicFireAtRelease::StopCooldown, 0.f, false, GetCooldownTime());
}

void UCMagicFireAtRelease::StopCooldown()
{
	bIsCoolDown = false;
}

UCMagicFireAtRelease::UCMagicFireAtRelease()
{
	bIsCoolDown = false;
}
