// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionAnimTimer.h"
#include "ActionAnimTimer_CastCharging.generated.h"

/**
 * 
 */
UCLASS()
class LWCASTLE_API UActionAnimTimer_CastCharging : public UCActionAnimTimer
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

protected:
	virtual void ExecuteAction(AActor* InstigatorActor) override;
	virtual void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
