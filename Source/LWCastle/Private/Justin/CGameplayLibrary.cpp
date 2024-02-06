// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameplayLibrary.h"
#include "GameplayTagContainer.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/CGameModeBase.h"
#include "CAIController.h"
#include "Justin/AComponents/CInventoryComponent.h"

bool UCGameplayLibrary::ApplyDamage(AActor * Invoker, AActor * AppliedActor, float Damage)
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
					Base->SlowDownTime(AppliedActor);
				}
			}
			else if (AppliedAttComp->IsAlive())
			{
				AppliedAttComp->ApplyDamage(Invoker, Damage);
				return true;
			}
		}
	}
	return false;
}

bool UCGameplayLibrary::ApplyStunOn(AActor * Invoker, AActor * AppliedActor, FGameplayTagContainer StunTag)
{
	auto OtherGameplayComp = AppliedActor->GetComponentByClass<UCGameplayComponent>();

	if (OtherGameplayComp)
	{
		FVector VectorDir = Invoker->GetActorLocation() - AppliedActor->GetActorLocation();

		VectorDir.Normalize();
		FRotator Rotation = VectorDir.Rotation();
		Rotation.Roll = 0.f;

		AppliedActor->SetActorRotation(Rotation);

		if (StunTag.HasTagExact(FGameplayTag::RequestGameplayTag("State.Stun.Hard")))
		{
			OtherGameplayComp->StartActionByName(Invoker, "StunHard");
		}
		else if (StunTag.HasTagExact(FGameplayTag::RequestGameplayTag("State.Stun.Light")))
		{
			OtherGameplayComp->StartActionByName(Invoker, "StunLight");
		}
		return true;
	}

	auto AIController = Cast<ACAIController>(AppliedActor->GetInstigatorController());
	if (AIController)
	{
		if (StunTag.HasTagExact(FGameplayTag::RequestGameplayTag("State.Stun.Hard")))
		{
			AIController->SetStunState();
			return true;
		}
	}
	return false;
}

bool UCGameplayLibrary::AddCurrency(AActor* ActorWithInventory)
{
	if (ensure(ActorWithInventory))
	{
		auto Inventory = ActorWithInventory->GetComponentByClass<UCInventoryComponent>();
		if (Inventory)
		{
			Inventory->AddCurrency(5.f);
			return true;
		}
	}
	return false;
}
