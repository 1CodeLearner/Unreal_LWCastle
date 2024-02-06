// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionAnimTimer.h"
#include "CActionAnimTimer_StunLight.generated.h"

class UCAPAnimTimer_Dodge;

/**
 * 
 */
UCLASS()
class LWCASTLE_API UCActionAnimTimer_StunLight : public UCActionAnimTimer
{
	GENERATED_BODY()

public:
	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;

	virtual bool CanStart_Implementation(AActor* InstigatorActor, UCAction* StartingAction) const;

private:
	void OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted);
	
	virtual void ExecuteAction(AActor* InstigatorActor) override;
};
