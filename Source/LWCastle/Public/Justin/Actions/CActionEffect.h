// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "CActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class LWCASTLE_API UCActionEffect : public UCAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	
};
