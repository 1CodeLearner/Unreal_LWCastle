// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "AIState")
	void SetStunState();
	UFUNCTION(BlueprintImplementableEvent, Category = "AIState")
	void SetBattleState();
	UFUNCTION(BlueprintImplementableEvent, Category = "AIState")
	void SetStartState();
};
