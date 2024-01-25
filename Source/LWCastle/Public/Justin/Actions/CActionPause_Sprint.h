// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionPause.h"
#include "CActionPause_Sprint.generated.h"

class UCPlayerAttributeComp;
/**
 * 
 */
UCLASS()
class LWCASTLE_API UCActionPause_Sprint : public UCActionPause, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UCActionPause_Sprint();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;

	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	virtual bool CanStart_Implementation(AActor* InstigatorActor) const override; 

protected:
	UPROPERTY(EditDefaultsOnly, meta= (ClampMin="0.0"),  Category = "Sprint")
	float MaxSprintSpeed;
	UPROPERTY(EditDefaultsOnly, meta= (ClampMin="0.0"), Category = "Sprint")
	float StaminaSpendRate;

private:
	UPROPERTY()
	TObjectPtr<UCPlayerAttributeComp> AttributeComp;
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<UCAction> ActionEffectStunClass;

	UFUNCTION()
	void OnStaminaDepleted();
	UFUNCTION()
	void OnLand( const FHitResult& Hit);

	float PrevSpeed;
	bool StartTick;

	bool isSprinting;

	void StartRunning();
	void PauseRunning();
	void UnPauseRunning();
	void StopRunning();
};
