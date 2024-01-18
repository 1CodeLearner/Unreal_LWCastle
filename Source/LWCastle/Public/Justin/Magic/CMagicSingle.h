// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Magic/CMagic.h"
#include "CMagicSingle.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LWCASTLE_API UCMagicSingle : public UCMagic
{
	GENERATED_BODY()
public:
	virtual void Press_Implementation(AActor* InstigatorActor) override;
	virtual void Release_Implementation(AActor* InstigatorActor) override;
	virtual void Reset_Implementation(AActor* InstigatorActor) override;

	virtual void MagicExecute(AActor* InstigatorActor) override;

private:
	FTimerHandle CooldownHandle;
	FTimerDelegate CooldownDelegate;
	bool bIsCoolDown;
	UFUNCTION()
	void StopCooldown(AActor* InstigatorActor);
public:
	UCMagicSingle();
};
