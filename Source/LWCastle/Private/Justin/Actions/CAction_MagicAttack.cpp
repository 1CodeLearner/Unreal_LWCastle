// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_MagicAttack.h"

#include "Kismet/GameplayStatics.h"

void UCAction_MagicAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ExecuteMagicDelegate.BindUFunction(this, "ExecuteMagic", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(ExecuteMagicHandle, ExecuteMagicDelegate, FireRate, bIsLooping, FireDelay);

	if (!bIsLooping)
	{
		GetWorld()->GetTimerManager().PauseTimer(ExecuteMagicHandle);
	}

}

void UCAction_MagicAttack::StopAction_Implementation(AActor* InstigatorActor)
{
	if (!bIsLooping)
	{
		GetWorld()->GetTimerManager().UnPauseTimer(ExecuteMagicHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
	}
	Super::StopAction_Implementation(InstigatorActor);
}

void UCAction_MagicAttack::ExecuteMagic(AActor* InstigatorActor)
{

	FVector Start = InstigatorActor->GetActorLocation();
	FVector End = Start + 2000.f * InstigatorActor->GetActorForwardVector();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorActor);
	FHitResult Hit;
	FColor DebugColor;
	bool Success = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
	Success ? DebugColor = FColor::Red  : DebugColor = FColor::Blue;
	if (Success)
	{
		 DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 32, DebugColor, true, 3.0f);
	}
	DrawDebugLine(GetWorld(), Start, End, DebugColor, true, 3.f);
}
