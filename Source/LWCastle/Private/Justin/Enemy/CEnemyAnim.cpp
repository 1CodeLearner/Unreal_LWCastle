// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Enemy/CEnemyAnim.h"
#include "Justin/Enemy/CBossPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
void UCEnemyAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	bIsBossStage = false;
}

void UCEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	BossPawn = Cast<ACBossPawn>(GetOwningActor());
	BlackBoard = BossPawn->GetBlackBoard();
}

void UCEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BossPawn)
	{
		bIsBossStage = BlackBoard->GetValueAsBool("IsBossStage");
	}
}
