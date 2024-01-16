// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CBossController.generated.h"

/**
 * 
 */
UCLASS()
class LWCASTLE_API ACBossController : public AAIController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossAI")
	UBehaviorTree* BossBehaviorTree;

};
