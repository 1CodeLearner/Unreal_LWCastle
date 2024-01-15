// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "CAction_MagicAttack.generated.h"

/**
 *
 */
UCLASS(Abstract)
class LWCASTLE_API UCAction_MagicAttack : public UCAction
{
	GENERATED_BODY()
public:

	void StartAction_Implementation(AActor* InstigatorActor) override;
	void StopAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	void ExecuteMagic(AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	FColor DebugMagicColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	bool bIsLoopingMagic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	float FireDelay;
private:
	FTimerHandle ExecuteMagicHandle;
	FTimerDelegate ExecuteMagicDelegate;
};
