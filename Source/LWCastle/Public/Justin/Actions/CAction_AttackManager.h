// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionPause.h"
#include "Justin/AComponents/CCombatComponent.h"
#include "CAction_AttackManager.generated.h"

class UCAction_MagicAttack;
/**
 * 
 */
UCLASS(Abstract)
class LWCASTLE_API UCAction_AttackManager : public UCActionPause
{
	GENERATED_BODY()
public:

	UCAction_AttackManager();
	
	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	//virtual bool CanStart_Implementation(AActor* InstigatorActor) const override;

	virtual bool IsRunning() const override;

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "MagicManager")
	FElement ActiveElement;
	/*FOR TESTING - REMOVE THIS THING AFTER TESTING SWITCHING BETWEEN CHARGING AND DEFAULT ATTACKS*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MagicManager")
	bool bIsCharged;

	UFUNCTION()
	void OnElementSwitched(AActor* InstigatorActor, FElementData ElementData, FElement SwitchedElement);

	UFUNCTION()
	void OnChargeStateActivated(AActor* InstigatorActor, bool _bIsCharged);

private:
	UCMagic* GetActiveElement() const;
	void SetActiveElement(FElement NewActiveMagic);
	bool IsSameMagic(FElement SwitchedElement) const;
	UPROPERTY()
	ACharacter* Character;
	FRotator OriginalRotationRate;
	//void ResetElementTags(FGameplayTagContainer TagsToRemove);
};
