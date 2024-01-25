// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionPause.h"
#include "CActionEffect.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UCActionEffect : public UCActionPause
{
	GENERATED_BODY()
public:

	UFUNCTION()
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;

	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;

	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;

protected:

	//How long effect lasts
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0."), Category="EffectSettings")
	float DurationTime;
	//Intervals to Tick effect within duration
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0."), Category="EffectSettings")
	float IntervalTime;

	FTimerHandle DurationHandle;
	FTimerDelegate DurationDelegate;
	FTimerHandle IntervalHandle;
	FTimerDelegate IntervalDelegate;

	UFUNCTION()
	void DurationEnd(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "ActionEffect")
	float GetDuration() const;

	UFUNCTION(BlueprintCallable, Category = "ActionEffect")
	float GetIntervalTime() const;

	UFUNCTION()
	virtual void IntervalStart(AActor* InstigatorActor);

	//Remove Action when Duration reaches the end.
	UPROPERTY(EditDefaultsOnly, Category = "EffectSettings")
	bool bDeleteAtEnd;

public:
	UCActionEffect();

};
