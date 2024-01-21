// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "CActionEffect.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UCActionEffect : public UCAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;


protected:

	//How long effect lasts
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0."), Category="ActionEffect")
	float DurationTime;
	//Intervals to Tick effect within duration
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0."), Category="ActionEffect")
	float IntervalTime;

	FTimerHandle DurationHandle;
	FTimerDelegate DurationDelegate;
	FTimerHandle IntervalHandle;
	FTimerDelegate IntervalDelegate;

	UFUNCTION()
	void DurationEnd(AActor* InstigatorActor);

	UFUNCTION()
	virtual void IntervalStart(AActor* InstigatorActor);
};
