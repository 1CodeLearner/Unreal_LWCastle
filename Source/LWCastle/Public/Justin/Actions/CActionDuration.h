// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "CActionDuration.generated.h"

/**
 * 
 */
UCLASS()
class LWCASTLE_API UCActionDuration : public UCAction
{
	GENERATED_BODY()
	
public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;
	virtual bool IsRunning() const override;

	UFUNCTION(BlueprintCallable)
	float GetActionDuration() const;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bHasDuration;

private:
	
	UFUNCTION()
	void StopDuration(AActor* InstigatorActor);
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", AllowPrivateAccess = "true"), Category = "Action")
	float ActionDuration;
	FTimerHandle DurationHandle;
	FTimerDelegate DurationDelegate;
};
