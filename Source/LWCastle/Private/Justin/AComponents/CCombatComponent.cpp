// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"

UCCombatComponent::UCCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

FMagicAttackGroup UCCombatComponent::GetActiveMagic() const
{
	return { ActiveDefaultMagic, ActiveChargedMagic };
}

void UCCombatComponent::Initialize()
{
	if (ensureMsgf(OwningDefaultMagic.Num() == 0 && OwningChargedMagic.Num() == 0, TEXT("Can only Initialize once")))
	{
		if (OwningDefaultMagicClasses.Num() == 0 && OwningChargedMagicClasses.Num() == 0) {
			return;
		}

		for (auto DefaultMagicClass : OwningDefaultMagicClasses)
		{
			auto NewMagic = NewObject<UCAction_MagicAttack>(GetOwner(), DefaultMagicClass);
			if (NewMagic)
			{
				OwningDefaultMagic.Add(NewMagic);
			}
		}


		for (auto ChargedMagicClass : OwningChargedMagicClasses)
		{
			auto NewChargeMagic = NewObject<UCAction_MagicAttack>(GetOwner(), ChargedMagicClass);
			if (NewChargeMagic)
			{
				OwningChargedMagic.Add(NewChargeMagic);
			}
		}

		if (OwningDefaultMagic.Num() > 0)
			ActiveDefaultMagic = OwningDefaultMagic.HeapTop();
		if (OwningChargedMagic.Num() > 0)
			ActiveChargedMagic = OwningChargedMagic.HeapTop();
	}
}
