// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionEffect.h"
#include "CActionEffect_ChargedState.generated.h"

class UCChargeWidget;
/**
 *
 */
UCLASS()
class LWCASTLE_API UCActionEffect_ChargedState : public UCActionEffect, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UCActionEffect_ChargedState();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;
	

	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

private:
	UPROPERTY()
	TObjectPtr<UCChargeWidget> Widget;
	bool StartTick = false;
};
