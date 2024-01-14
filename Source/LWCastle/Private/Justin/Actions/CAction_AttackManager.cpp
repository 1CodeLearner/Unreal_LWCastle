// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_AttackManager.h"

#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"

void UCAction_AttackManager::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	if(!ActiveMagicAttack)
	{
		AActor* OwningActor = Cast<AActor>(GetOuter());
		if(OwningActor)
		{
			auto CombatComp = OwningActor->GetComponentByClass<UCCombatComponent>();
			if(CombatComp)
			{
				ActiveMagicAttack = CombatComp->GetActiveMagic();
				ensure(ActiveMagicAttack->GetGameplayComponent() == nullptr);
				CombatComp->OnActiveMagicSwitched.AddDynamic(this, &UCAction_AttackManager::OnWeaponSwitched);
			}
		}
	}
	ActiveMagicAttack->StartAction(InstigatorActor);
}

void UCAction_AttackManager::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	ActiveMagicAttack->StopAction(InstigatorActor);
}

void UCAction_AttackManager::OnWeaponSwitched(AActor* InstigatorActor, UCAction_MagicAttack* SwitchedMagicAttack)
{
	ensure(SwitchedMagicAttack->GetOuter() == GetOuter() && SwitchedMagicAttack->GetGameplayComponent() == nullptr);

	if (ActiveMagicAttack == SwitchedMagicAttack)
		return;

	if (ActiveMagicAttack->IsRunning())
	{
		ActiveMagicAttack->StopAction(InstigatorActor);
		ActiveMagicAttack = SwitchedMagicAttack;
		ActiveMagicAttack->StartAction(InstigatorActor);
	}

	ActiveMagicAttack = SwitchedMagicAttack;
}
