// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "CAction_Sprint.generated.h"

class UCPlayerAttributeComp;
class UCActionEffect;
/**
 *
 */
UCLASS()
class LWCASTLE_API UCAction_Sprint : public UCAction, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UCAction_Sprint();
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;

	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;
protected:
	float MaxSprintSpeed = 800;
	UPROPERTY(EditDefaultsOnly, meta= (ClampMin="0.0"), Category = "Sprint")
	float StaminaSpendRate;

private:
	UPROPERTY()
	TObjectPtr<UCPlayerAttributeComp> AttributeComp;
	bool StartTick;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<UCAction> ActionEffectStunClass;
	UFUNCTION()
	void OnStaminaDepleted();

};
