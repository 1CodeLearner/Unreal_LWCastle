// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_Sprint.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"


UCAction_Sprint::UCAction_Sprint()
{
	StartTick = false;
	StaminaSpendRate = 1.0f;
}

void UCAction_Sprint::Tick(float DeltaTime)
{
	if (AttributeComp)
	{
		AttributeComp->SpendStamina(StaminaSpendRate * DeltaTime);
	}
}

TStatId UCAction_Sprint::GetStatId() const
{
	TStatId Stat;
	return Stat;
}

bool UCAction_Sprint::IsTickable() const
{
	return true;
}

bool UCAction_Sprint::IsAllowedToTick() const
{
	return StartTick;
}

void UCAction_Sprint::OnStaminaDepleted()
{
	StartTick = false; 
	GetGameplayComponent()->AddAction(GetGameplayComponent()->GetOwner(), ActionEffectStunClass);
}

void UCAction_Sprint::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	auto Attribute = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
	if (Attribute)
	{
		AttributeComp = Attribute;
	}
}

void UCAction_Sprint::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	AttributeComp->OnStaminaDepleted.AddDynamic(this, &UCAction_Sprint::OnStaminaDepleted);
	StartTick = true;
}

void UCAction_Sprint::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	AttributeComp->OnStaminaDepleted.Remove(this, "OnStaminaDepleted");
	StartTick = false;
}

void UCAction_Sprint::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	AttributeComp->OnStaminaDepleted.Remove(this, "OnStaminaDepleted");
	StartTick = false;
}
