// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/CActionEffectInterface.h"
#include "Justin/Actions/CAction.h"
#include "CActionEffect.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UCActionEffect : public UCAction, public ICActionEffectInterface
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
	virtual bool CanPause(AActor* InstigatorActor, UCAction* OtherAction) const override;

	UFUNCTION(BlueprintCallable)
	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(BlueprintCallable)
	virtual bool CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const override;

	UFUNCTION(BlueprintCallable)
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	virtual bool IsPausing() const override;

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

	//Pauses & unpauses existing action in GamplayComponent if same tag exists in Action that is about to Start or Stop.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer PausedTags;

	UFUNCTION()
	void DurationEnd(AActor* InstigatorActor);

	UFUNCTION()
	virtual void IntervalStart(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, Category = "ActionEffect")
	bool bCanPause;

	//Remove Action when Duration reaches the end.
	UPROPERTY(EditDefaultsOnly, Category = "ActionEffect")
	bool bDeleteAtEnd;

public:
	UCActionEffect();

private:
	bool bIsPausing;
};
