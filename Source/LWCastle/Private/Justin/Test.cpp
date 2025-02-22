// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Test.h"

#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CCombatComponent.h"
// Sets default values
ATest::ATest()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerAttributeComp = CreateDefaultSubobject<UCPlayerAttributeComp>("PlayerAttributeComp");
	GameplayComp = CreateDefaultSubobject<UCGameplayComponent>("GameplayComp");
	CombatComp = CreateDefaultSubobject<UCCombatComponent>("CombatComp");
}

void ATest::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CombatComp->Initialize();
}

// Called when the game starts or when spawned
void ATest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

