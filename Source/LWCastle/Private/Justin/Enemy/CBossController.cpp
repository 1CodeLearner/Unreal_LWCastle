// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Enemy/CBossController.h"

void ACBossController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BossBehaviorTree);
}
