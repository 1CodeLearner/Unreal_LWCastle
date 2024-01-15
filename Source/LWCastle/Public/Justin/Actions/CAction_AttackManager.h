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
	UFUNCTION()
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;
	bool CanInterrupt_Implementation(AActor* InstigatorActor, FGameplayTagContainer OtherGrantedTag) const override;

protected:
	//UPROPERTY(VisibleAnywhere, Category = "MagicManager")
	//TObjectPtr<UCAction_MagicAttack> ActiveDefaultAttack;
	
	UPROPERTY(VisibleAnywhere, Category = "MagicManager")
	FMagicAttackGroup ActiveMagic;
	/*FOR TESTING - REMOVE THIS THING AFTER TESTING SWITCHING BETWEEN CHARGING AND DEFAULT ATTACKS*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MagicManager")
	bool bIsDefaultMagic;

	UFUNCTION()
	void OnWeaponSwitched(AActor* InstigatorActor, FMagicAttackGroup ActiveMagicGroup);

private:
	UCAction_MagicAttack* GetActiveMagic() const;
	void SetActiveMagic(FMagicAttackGroup NewActiveMagic);
	bool IsSameMagic(FMagicAttackGroup ActiveMagicGroup) const;
};
