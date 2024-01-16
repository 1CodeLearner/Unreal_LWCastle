// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Enemy/CEnemyCharacter.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CEnemyAttributeComp.h"

void ACEnemyCharacter::StartBoss()
{
	bIsBossStage = true;
}


ACEnemyCharacter::ACEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsBossStage = false;
	GameplayComp = CreateDefaultSubobject<UCGameplayComponent>(TEXT("GameplayComp"));
	AttributeComp = CreateDefaultSubobject<UCEnemyAttributeComp>("AttributeComp");
}

void ACEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

