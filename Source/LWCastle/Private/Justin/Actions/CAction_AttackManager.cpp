// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_AttackManager.h"

#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"
#include "Justin/Magic/CMagic.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Uwol/uwol_test.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCAction_AttackManager::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	GetActiveElement()->Press(InstigatorActor);



	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;

}

void UCAction_AttackManager::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
	//Magic will remain paused forever if not set to true
	GetActiveElement()->Reset(InstigatorActor, true);

	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UCAction_AttackManager::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	if (IsRunning())
	{
		GetActiveElement()->Press(InstigatorActor);
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void UCAction_AttackManager::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	GetActiveElement()->Release(InstigatorActor);

	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

/*
bool UCAction_AttackManager::CanStart_Implementation(AActor* InstigatorActor) const
{
	return Super::CanStart_Implementation(InstigatorActor);
	if (GetActiveElement())
		return !GetActiveElement()->IsPressing();
	return false;
}*/

bool UCAction_AttackManager::IsRunning() const
{
	if (GetActiveElement())
		return GetActiveElement()->IsPressing();
	return false;
}

UCAction_AttackManager::UCAction_AttackManager()
{
	bIsCharged = false;
}

void UCAction_AttackManager::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	//Initialize Magic attack for player
	if (!ActiveElement.DefaultElement)
	{
		AActor* OwningActor = Cast<AActor>(GetOuter());
		if (OwningActor)
		{
			auto CombatComp = OwningActor->GetComponentByClass<UCCombatComponent>();
			Character = Cast<Auwol_test>(OwningActor);

			if (CombatComp && ensure(Character))
			{
				ActiveElement = CombatComp->GetActiveElement();

				if (ensure(ActiveElement.DefaultElement && ActiveElement.ChargedElement))
				{
					CombatComp->OnActiveElementSwitched.AddDynamic(this, &UCAction_AttackManager::OnElementSwitched);
					CombatComp->OnChargeStateActivated.AddDynamic(this, &UCAction_AttackManager::OnChargeStateActivated);
				}
			}
		}
	}
}

//void UCAction_AttackManager::ResetElementTags(FGameplayTagContainer TagsToRemove)
//{
//	auto Gameplay = GetGameplayComponent();
//	if (Gameplay)
//	{
//		Gameplay->ActiveGameplayTags.RemoveTags(GrantedTags);
//	}
//}

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
			//ResetElementTags(GetActiveElement()->GetGrantedTags());

			Super::CompleteAction(InstigatorActor);

			SetActiveElement(SwitchedElement);

			StartAction(InstigatorActor);

			//GetActiveElement()->Press(InstigatorActor);
			//CompleteAction(InstigatorActor);
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
	if (GetGameplayComponent())
	{
		static FGameplayTag ChargedStateTag = FGameplayTag::RequestGameplayTag("State.Charged");
		if (GetGameplayComponent()->ActiveGameplayTags.HasTagExact(ChargedStateTag))
			return ActiveElement.ChargedElement;
		else
			return ActiveElement.DefaultElement;
	}
	return nullptr;
}

void UCAction_AttackManager::SetActiveElement(FElement SwitchedElement)
{
	ActiveElement = SwitchedElement;
}

bool UCAction_AttackManager::IsSameMagic(FElement SwitchedElement) const
{
	return SwitchedElement.DefaultElement == ActiveElement.DefaultElement;
}
