// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"

UCCombatComponent::UCCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UCCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto DefaultMagicClass : OwningDefaultMagicClasses)
	{
		auto NewMagic = NewObject<UCAction_MagicAttack>(GetOwner(), DefaultMagicClass);
		if (NewMagic)
		{
			OwningDefaultMagic.Add(NewMagic);
		}
	}
	if(OwningDefaultMagic.Num() > 0)
		ActiveDefaultMagic = OwningDefaultMagic.HeapTop();
}


FMagicAttackGroup UCCombatComponent::GetActiveMagic() const
{
	return { ActiveDefaultMagic, ActiveDefaultMagic };
}
