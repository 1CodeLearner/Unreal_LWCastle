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
	virtual bool CanStart_Implementation(AActor* InstigatorActor) const override;

	virtual void Initialize(UCGameplayComponent* GameplayComp) override;
	virtual bool IsRunning() const override;

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "MagicManager")
	FMagicAttackGroup ActiveMagic;
	/*FOR TESTING - REMOVE THIS THING AFTER TESTING SWITCHING BETWEEN CHARGING AND DEFAULT ATTACKS*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MagicManager")
	bool bIsCharged;

	UFUNCTION()
	void OnWeaponSwitched(AActor* InstigatorActor, FMagicAttackGroup ActiveMagicGroup);

	UFUNCTION()
	void OnChargeStateActivated(bool _bIsCharged);

private:
	UCAction_MagicAttack* GetActiveMagic() const;
	void SetActiveMagic(FMagicAttackGroup NewActiveMagic);
	bool IsSameMagic(FMagicAttackGroup ActiveMagicGroup) const;
};
