// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameplayLibrary.h"
#include "GameplayTagContainer.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/CGameModeBase.h"

void UCGameplayLibrary::ApplyDamage(AActor* Invoker, AActor* AppliedActor, float Damage)
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("State.Dodge");

	if (ensure(Invoker && AppliedActor))
	{
		UCAttributeComponent* AppliedAttComp = AppliedActor->GetComponentByClass<UCAttributeComponent>();
		UCGameplayComponent* AppliedGameplayComp = AppliedActor->GetComponentByClass<UCGameplayComponent>();
		if (AppliedAttComp)
		{
			if (AppliedGameplayComp && AppliedGameplayComp->ActiveGameplayTags.HasTagExact(Tag)) 
			{
				auto GameMode = UGameplayStatics::GetGameMode(Invoker);
				ACGameModeBase* Base = Cast<ACGameModeBase>(GameMode);
				if (Base) 
				{
					Base->SlowDownTime(Invoker);
				}
				return;
			}

			if (AppliedAttComp->IsAlive())
			{
				AppliedAttComp->ApplyDamage(Invoker, Damage);
			}
		}
	}
}