// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_AttackManager.h"

#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"
#include "Justin/Magic/CMagic.h"

void UCAction_AttackManager::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	GetActiveElement()->Press(InstigatorActor);
}

void UCAction_AttackManager::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	GetActiveElement()->Release(InstigatorActor);
}


bool UCAction_AttackManager::CanStart_Implementation(AActor* InstigatorActor) const
{
	return !GetActiveElement()->IsPressing();
}

bool UCAction_AttackManager::IsRunning() const
{
	return GetActiveElement()->IsPressing();
}


UCAction_AttackManager::UCAction_AttackManager()
{
	bIsCharged = false;
}

void UCAction_AttackManager::Initialize(UCGameplayComponent* GameplayComp)
{
	Super::Initialize(GameplayComp);

	//Initialize Magic attack for player
	if (!ActiveElement.DefaultElement)
	{
		AActor* OwningActor = Cast<AActor>(GetOuter());
		if (OwningActor)
		{
			auto CombatComp = OwningActor->GetComponentByClass<UCCombatComponent>();
			if (CombatComp)
			{
				ActiveElement = CombatComp->GetActiveElement();

				if (ensure(ActiveElement.DefaultElement&& ActiveElement.ChargedElement))
				{
					CombatComp->OnActiveElementSwitched.AddDynamic(this, &UCAction_AttackManager::OnElementSwitched);
					CombatComp->OnChargeStateActivated.AddDynamic(this, &UCAction_AttackManager::OnChargeStateActivated);
				}
			}
		}
	}
}

void UCAction_AttackManager::OnElementSwitched(AActor* InstigatorActor, FElementData ElementData, FElement SwitchedElement)
{
	if (ensure(SwitchedElement.DefaultElement && SwitchedElement.ChargedElement))
	{
		ensure(SwitchedElement.ChargedElement->GetOuter() == GetOuter());
		ensure(SwitchedElement.DefaultElement->GetOuter() == GetOuter());

		//ensureAlwaysMsgf(GetActiveMagic() == ActiveMagic.ActiveDefaultMagic, TEX("Cannot Switch weapon During Charged Magic State!!!"));

		if (IsSameMagic(SwitchedElement))
			return;

		if (GetActiveElement()->IsPressing())
		{
			GetActiveElement()->Reset(InstigatorActor);
			SetActiveElement(SwitchedElement);
			GetActiveElement()->Press(InstigatorActor);
		}
		SetActiveElement(SwitchedElement);
	}
}

void UCAction_AttackManager::OnChargeStateActivated(AActor* InstigatorActor, bool _bIsCharged)
{
	if (bIsCharged != _bIsCharged) {
		if (GetActiveElement()->IsPressing()) 
		{
			GetActiveElement()->Reset(InstigatorActor);
		}
		bIsCharged = _bIsCharged;
	}
}

UCMagic* UCAction_AttackManager::GetActiveElement() const
{
	if(bIsCharged)
		return ActiveElement.ChargedElement;
	else
		return ActiveElement.DefaultElement;
}

void UCAction_AttackManager::SetActiveElement(FElement SwitchedElement)
{
	ActiveElement = SwitchedElement;
}

bool UCAction_AttackManager::IsSameMagic(FElement SwitchedElement) const
{
	return SwitchedElement.DefaultElement == ActiveElement.DefaultElement;
}
