// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CEnemyAnim.generated.h"

class ACBossPawn;
class UBlackboardComponent;
/**
 *
 */
UCLASS()
class LWCASTLE_API UCEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossAnimation")
	TObjectPtr<ACBossPawn> BossPawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossAnimation")
	TObjectPtr<UBlackboardComponent> BlackBoard;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossAnimation")
	bool bIsBossStage;
};
