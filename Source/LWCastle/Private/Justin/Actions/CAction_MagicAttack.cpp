// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_MagicAttack.h"

#include "Justin/CGameplayLibrary.h"
#include "Kismet/GameplayStatics.h"

void UCAction_MagicAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (bIsLoopingMagic)
	{
		ExecuteMagicDelegate.BindUFunction(this, "ExecuteMagic", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(ExecuteMagicHandle, ExecuteMagicDelegate, FireRate, bIsLoopingMagic, FireDelay);
	}
	else
	{
		if (bIsMagicCooldown)
		{
			return;
		}

		ensure(!GetWorld()->GetTimerManager().IsTimerActive(CooldownMagicHandle));

		SetupExecuteMagic(InstigatorActor);
	}

}

void UCAction_MagicAttack::StopAction_Implementation(AActor* InstigatorActor)
{
	if (bIsLoopingMagic)
	{
		GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
		Super::StopAction_Implementation(InstigatorActor);

	}
	else
	{
		if (bIsMagicCooldown)
		{
			return;
		}

		if (ExecuteMagicHandle.IsValid()) {
			GetWorld()->GetTimerManager().UnPauseTimer(ExecuteMagicHandle);
		}
		else
		{
			Super::StopAction_Implementation(InstigatorActor);
		}
	}
}


void UCAction_MagicAttack::SetupExecuteMagic(AActor* InstigatorActor)
{
	ExecuteMagicDelegate.BindUFunction(this, "ExecuteMagic", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(ExecuteMagicHandle, ExecuteMagicDelegate, FireRate, bIsLoopingMagic, FireDelay);
	GetWorld()->GetTimerManager().PauseTimer(ExecuteMagicHandle);
}

void UCAction_MagicAttack::ExecuteMagic(AActor* InstigatorActor)
{
	if (ensure(InstigatorActor))
	{
		FVector Origin = InstigatorActor->GetActorLocation();
		FVector Start = Origin + 100.f * InstigatorActor->GetActorForwardVector();
		FVector End = Start + 2000.f * InstigatorActor->GetActorForwardVector();
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorActor);
		FHitResult Hit;
		FColor DebugColorLocal;
		bool Success = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
		if (Success)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 32, FColor::Red, false, 3.0f);
			UCGameplayLibrary::ApplyDamage(InstigatorActor, Hit.GetActor());

		}
		DrawDebugLine(GetWorld(), Start, End, this->DebugMagicColor, false, 5.f, DebugLineThickness);

		if (!bIsLoopingMagic)
		{
			CooldownMagicDelegate.BindUFunction(this, "StopCooldown", InstigatorActor);
			GetWorld()->GetTimerManager().SetTimer(CooldownMagicHandle, CooldownMagicDelegate, 0.5f, false, FireRate);
			bIsMagicCooldown = true;
			GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
		}
	}
}

void UCAction_MagicAttack::StopCooldown(AActor* InstigatorActor)
{
	bIsMagicCooldown = false;
	GetWorld()->GetTimerManager().ClearTimer(CooldownMagicHandle);
	StopAction(InstigatorActor);
}
