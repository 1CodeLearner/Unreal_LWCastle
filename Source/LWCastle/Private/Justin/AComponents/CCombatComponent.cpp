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

	for (auto MagicClass : OwningMagicClasses)
	{
		auto NewMagic = NewObject<UCAction_MagicAttack>(GetOwner(), MagicClass);
		if (NewMagic)
		{
			OwningMagic.Add(NewMagic);
		}
	}
	if(OwningMagic.Num() > 0)
		ActiveMagic = OwningMagic.HeapTop();
}


UCAction_MagicAttack* UCCombatComponent::GetActiveMagic() const
{
	return ActiveMagic;
}
