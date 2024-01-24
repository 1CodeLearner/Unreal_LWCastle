// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionAnimTimer.h"
#include "Justin/Widgets/CChargeWidget.h"
#include "ActionAnimTimer_CastCharging.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UActionAnimTimer_CastCharging : public UCActionAnimTimer, public FTickableGameObject
{
	GENERATED_BODY()
public:
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;

	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	virtual void PauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void UnPauseAction_Implementation(AActor* InstigatorActor) override;
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

protected:
	virtual void ExecuteAction(AActor* InstigatorActor) override;
	virtual void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;

private:
	UPROPERTY()
	TObjectPtr<UCChargeWidget> Widget;
	UPROPERTY()
	TObjectPtr<UCPlayerAttributeComp> PlayerAttribute;
	bool StartTick = false;
};
