// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_AttackManager.h"

#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"

void UCAction_AttackManager::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	UCAction_MagicAttack* DefaultMagic = ActiveMagic.ActiveDefaultMagic;
	UCAction_MagicAttack* ChargeMagic = ActiveMagic.ActiveDefaultMagic;

	if (!DefaultMagic && !ChargeMagic)
	{
		AActor* OwningActor = Cast<AActor>(GetOuter());
		if (OwningActor)
		{
			auto CombatComp = OwningActor->GetComponentByClass<UCCombatComponent>();
			if (CombatComp)
			{
				ActiveMagic.ActiveDefaultMagic = CombatComp->GetActiveMagic().ActiveDefaultMagic;
				ActiveMagic.ActiveChargeMagic = CombatComp->GetActiveMagic().ActiveChargeMagic;

				if (ensure(ActiveMagic.ActiveDefaultMagic && ActiveMagic.ActiveChargeMagic))
				{
					ensure(ActiveMagic.ActiveDefaultMagic->GetGameplayComponent() == nullptr
						&& ActiveMagic.ActiveChargeMagic->GetGameplayComponent() == nullptr);

					CombatComp->OnActiveMagicSwitched.AddDynamic(this, &UCAction_AttackManager::OnWeaponSwitched);
				}
			}
		}
	}

	if (GetActiveMagic() != nullptr && GetActiveMagic()->CanStart(InstigatorActor))
		GetActiveMagic()->StartAction(InstigatorActor);
}

void UCAction_AttackManager::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	GetActiveMagic()->StopAction(InstigatorActor);
}

bool UCAction_AttackManager::CanInterrupt_Implementation(AActor* InstigatorActor, FGameplayTagContainer OtherGrantedTag) const
{
	return true;
	if (bCanInterrupt) 
	{
		
	}
}

void UCAction_AttackManager::OnWeaponSwitched(AActor* InstigatorActor, FMagicAttackGroup ActiveMagicGroup)
{
	if (ensure(ActiveMagicGroup.ActiveChargeMagic && ActiveMagicGroup.ActiveDefaultMagic))
	{
		ensure(ActiveMagicGroup.ActiveChargeMagic->GetOuter() == GetOuter() && ActiveMagicGroup.ActiveChargeMagic->GetGameplayComponent() == nullptr);
		ensure(ActiveMagicGroup.ActiveDefaultMagic->GetOuter() == GetOuter() && ActiveMagicGroup.ActiveDefaultMagic->GetGameplayComponent() == nullptr);

		//ensureAlwaysMsgf(GetActiveMagic() == ActiveMagic.ActiveDefaultMagic, TEXT("Cannot Switch weapon During Charged Magic State!!!"));

		if (IsSameMagic(ActiveMagicGroup))
			return;

		if (GetActiveMagic()->IsRunning())
		{
			GetActiveMagic()->StopAction(InstigatorActor);
			SetActiveMagic(ActiveMagicGroup);
			GetActiveMagic()->StartAction(InstigatorActor);
		}

		SetActiveMagic(ActiveMagicGroup);
	}
}

UCAction_MagicAttack* UCAction_AttackManager::GetActiveMagic() const
{
	if (bIsDefaultMagic)
	{
		return ActiveMagic.ActiveDefaultMagic;
	}
	else
	{
		return ActiveMagic.ActiveChargeMagic;
	}

}
void UCAction_AttackManager::SetActiveMagic(FMagicAttackGroup NewActiveMagic)
{
	ActiveMagic.ActiveDefaultMagic = NewActiveMagic.ActiveDefaultMagic;
	ActiveMagic.ActiveChargeMagic = NewActiveMagic.ActiveChargeMagic;
}

bool UCAction_AttackManager::IsSameMagic(FMagicAttackGroup ActiveMagicGroup) const
{
	return ActiveMagicGroup.ActiveDefaultMagic == ActiveMagic.ActiveDefaultMagic;
}
