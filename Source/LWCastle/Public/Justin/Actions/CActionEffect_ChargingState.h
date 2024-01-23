// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionEffect.h"
#include "CActionEffect_ChargingState.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UCActionEffect_ChargingState : public UCActionEffect
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;

};
