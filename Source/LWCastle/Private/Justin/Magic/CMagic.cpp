// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagic.h"
#include "Justin/CGameplayLibrary.h"

void UCMagic::Press_Implementation(AActor* InstigatorActor)
{
	bIsPressing = true;
	UE_LOG(LogTemp, Warning, TEXT("Running Press %s"), *GetNameSafe(this));
}

void UCMagic::Release_Implementation(AActor* InstigatorActor)
{
	bIsPressing = false;
	UE_LOG(LogTemp, Warning, TEXT("Running Release %s"), *GetNameSafe(this));
}

void UCMagic::Reset_Implementation(AActor* InstigatorActor)
{
	bIsPressing = false;
	if (MagicHandle.IsValid())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	UE_LOG(LogTemp, Warning, TEXT("Running Reset %s"), *GetNameSafe(this));
}

void UCMagic::MagicExecute(AActor* InstigatorActor)
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
}

UCMagic::UCMagic()
{
	DelayTime = 0;
	bIsPressing = false;
}

float UCMagic::GetDelayTime() const
{
	return DelayTime;
}

bool UCMagic::IsPressing() const
{
	return bIsPressing;
}

UWorld* UCMagic::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}