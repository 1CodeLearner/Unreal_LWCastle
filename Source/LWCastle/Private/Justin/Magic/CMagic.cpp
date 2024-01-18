// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagic.h"
#include "Justin/CGameplayLibrary.h"

void UCMagic::Press_Implementation(AActor* InstigatorActor)
{
	bIsPressed = true;
}

void UCMagic::Release_Implementation(AActor* InstigatorActor)
{
	bIsPressed = false;
}

void UCMagic::Reset_Implementation(AActor* InstigatorActor)
{
	bIsPressed = false;
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
	bIsPressed = false;
}

float UCMagic::GetDelayTime() const
{
	return DelayTime;
}

bool UCMagic::IsPressed() const
{
	return bIsPressed;
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