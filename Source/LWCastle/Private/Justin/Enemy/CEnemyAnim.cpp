// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Enemy/CEnemyAnim.h"
#include "Justin/Enemy/CEnemyCharacter.h"

void UCEnemyAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	bIsBossStage = false;
}

void UCEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	BossCharacter = Cast<ACEnemyCharacter>(GetOwningActor());
}

void UCEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BossCharacter) 
	{
		bIsBossStage = BossCharacter->bIsBossStage;
	}
}
