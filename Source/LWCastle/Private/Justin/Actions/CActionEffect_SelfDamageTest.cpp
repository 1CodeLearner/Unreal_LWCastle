// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionEffect_SelfDamageTest.h"

#include "Justin/CGameplayLibrary.h"
#include "Justin/AComponents/CAttributeComponent.h"


void UCActionEffect_SelfDamageTest::IntervalStart(AActor* InstigatorActor)
{


	UCGameplayLibrary::ApplyDamage(InstigatorActor, Cast<AActor>(GetOuter()), DamageAmount);
}
