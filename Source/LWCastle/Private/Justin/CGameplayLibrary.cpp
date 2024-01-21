// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameplayLibrary.h"

#include "Justin/AComponents/CAttributeComponent.h"

void UCGameplayLibrary::ApplyDamage(AActor* Invoker, AActor* AppliedActor, float Damage)
{
	if (ensure(Invoker && AppliedActor))
	{
		UCAttributeComponent* InvokerComp = Invoker->GetComponentByClass<UCAttributeComponent>();
		UCAttributeComponent* AppliedComp = AppliedActor->GetComponentByClass<UCAttributeComponent>();
		if (AppliedComp && InvokerComp)
		{
			if (AppliedComp->IsAlive())
			{
				
				AppliedComp->ApplyDamage(Invoker, Damage);
			}
		}
	}
}
