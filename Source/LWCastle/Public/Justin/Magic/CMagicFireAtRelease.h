// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Magic/CMagic.h"
#include "CMagicFireAtRelease.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LWCASTLE_API UCMagicFireAtRelease : public UCMagic
{
	GENERATED_BODY()
public:
	virtual void Press_Implementation(AActor* InstigatorActor) override;
	virtual void Release_Implementation(AActor* InstigatorActor) override;
	virtual void MagicExecute(AActor* InstigatorActor) override;
};
