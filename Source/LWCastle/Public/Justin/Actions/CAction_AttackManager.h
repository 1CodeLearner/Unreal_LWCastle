// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "Justin/AComponents/CCombatComponent.h"
#include "CAction_AttackManager.generated.h"

class UCAction_MagicAttack;
/**
 * 
 */
UCLASS()
class LWCASTLE_API UCAction_AttackManager : public UCAction
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "MagicManager")
	TObjectPtr<UCAction_MagicAttack> ActiveDefaultAttack;
	
	UPROPERTY(VisibleAnywhere, Category = "MagicManager")
	TObjectPtr<UCAction_MagicAttack> ActiveChargeAttack;

	UFUNCTION()
	void OnWeaponSwitched(AActor* InstigatorActor, UCAction_MagicAttack* SwitchedMagicAttack);
};
