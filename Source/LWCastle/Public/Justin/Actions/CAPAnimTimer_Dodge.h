// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAPAnimTimer.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "CAPAnimTimer_Dodge.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LWCASTLE_API UCAPAnimTimer_Dodge : public UCAPAnimTimer
{
	GENERATED_BODY()

	friend class UCActionAnimTimer_StunLight;

public:
	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Roll")
	float StaminaSpendAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Roll")
	TSubclassOf<UCAction> StunActionClass;

private:
	void OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted);
	UPROPERTY()
	TObjectPtr<UCPlayerAttributeComp> AttComp;

	UFUNCTION()
	void OnStaminaDepleted();

	void AddBlockedTag();
	void RemoveBlockedTag();
};
