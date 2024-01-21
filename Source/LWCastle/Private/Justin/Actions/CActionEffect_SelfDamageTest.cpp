// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionEffect_SelfDamageTest.h"

#include "Justin/CGameplayLibrary.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/AComponents/CGameplayComponent.h"


void UCActionEffect_SelfDamageTest::IntervalStart(AActor* InstigatorActor)
{
	UCGameplayLibrary::ApplyDamage(InstigatorActor, Cast<AActor>(GetOuter()), DamageAmount);
}

void UCActionEffect_SelfDamageTest::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	UE_LOG(LogTemp, Warning, TEXT("LOOK MA IM ON TV!!!"));
}
