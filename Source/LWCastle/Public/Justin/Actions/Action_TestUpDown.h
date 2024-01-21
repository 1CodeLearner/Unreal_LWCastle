// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "Action_TestUpDown.generated.h"

class UCPlayerAttributeComp;
/**
 * 
 */
UCLASS()
class LWCASTLE_API UAction_TestUpDown : public UCAction
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;
	//virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(BlueprintCallable)
	void ResetMagic(AActor* InstigatorActor);

protected:

	UPROPERTY()
	UCPlayerAttributeComp* PlayerAttComp;

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteMagic(AActor* InstigatorActor);
	FTimerHandle ExecuteMagicHandle;
	FTimerDelegate ExecuteMagicDelegate;
};
