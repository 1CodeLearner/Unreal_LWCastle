// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAPAnimTimer.h"
#include "CAPAnimTimer_Dodge.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LWCASTLE_API UCAPAnimTimer_Dodge : public UCAPAnimTimer
{
	GENERATED_BODY()
public:
	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;

private:
	void OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted);
};
