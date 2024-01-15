// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_MagicAttack.h"

#include "Kismet/GameplayStatics.h"

void UCAction_MagicAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ensure(!GetWorld()->GetTimerManager().IsTimerActive(CooldownMagicHandle) && bIsMagicCooldown == false);

	ExecuteMagicDelegate.BindUFunction(this, "ExecuteMagic", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(ExecuteMagicHandle, ExecuteMagicDelegate, FireRate, bIsLoopingMagic, FireDelay);

	if (!bIsLoopingMagic)
	{
		GetWorld()->GetTimerManager().PauseTimer(ExecuteMagicHandle);
	}
}

void UCAction_MagicAttack::StopAction_Implementation(AActor* InstigatorActor)
{
	if (!bIsLoopingMagic)
	{
		if (bIsMagicCooldown)
		{
			bIsMagicCooldown = false;
			Super::StopAction_Implementation(InstigatorActor);
		}
		else {
			GetWorld()->GetTimerManager().UnPauseTimer(ExecuteMagicHandle);
			bIsMagicCooldown = true;
			CooldownMagicDelegate.BindUFunction(this, "StopAction", InstigatorActor);
			GetWorld()->GetTimerManager().SetTimer(CooldownMagicHandle, CooldownMagicDelegate, 0.f, false, FireRate);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
		Super::StopAction_Implementation(InstigatorActor);
	}
}

void UCAction_MagicAttack::ExecuteMagic(AActor* InstigatorActor)
{

	FVector Start = InstigatorActor->GetActorLocation();
	FVector End = Start + 2000.f * InstigatorActor->GetActorForwardVector();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorActor);
	FHitResult Hit;
	FColor DebugColorLocal;
	bool Success = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
	Success ? DebugColorLocal = FColor::Red  : DebugColorLocal = FColor::Blue;
	if (Success)
	{
		 DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 32, DebugColorLocal, false, 3.0f);
	}
	DrawDebugLine(GetWorld(), Start, End, this->DebugMagicColor, false, 5.f, DebugLineThickness);
}
