// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicFireAtRelease.h"

void UCMagicFireAtRelease::Press_Implementation(AActor* InstigatorActor)
{
	Super::Press_Implementation(InstigatorActor);
	ClearNotifyBinds();
}

void UCMagicFireAtRelease::Release_Implementation(AActor* InstigatorActor)
{
	Super::Release_Implementation(InstigatorActor);
	if(IsMontagePlaying())
		return;

	StartMontage();
}

void UCMagicFireAtRelease::MagicExecute_Implementation(AActor* InstigatorActor)
{
	Super::MagicExecute_Implementation(InstigatorActor);
	UE_LOG(LogTemp, Warning, TEXT("MagicExecute in MagicFireAtRelease"));

	UAnimInstance* Anim = GetAnimInstance();

	if (!Anim)
		return;

	if (Anim->OnPlayMontageNotifyBegin.Contains(this, "OnNotifyBegin"))
	{
		Anim->OnPlayMontageNotifyBegin.Remove(this, "OnNotifyBegin");
	}
}
