// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Enemy/CBossPawn.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CEnemyAttributeComp.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ACBossPawn::ACBossPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameplayComp = CreateDefaultSubobject<UCGameplayComponent>(TEXT("GameplayComp"));
	AttributeComp = CreateDefaultSubobject<UCEnemyAttributeComp>("AttributeComp");

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
}

void ACBossPawn::StartBoss()
{
	GetBlackBoard()->SetValueAsBool("IsBossStage", true);
}

// Called when the game starts or when spawned
void ACBossPawn::BeginPlay()
{
	Super::BeginPlay();

	GetBlackBoard()->SetValueAsObject("TargetActor", UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void ACBossPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACBossPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBlackboardComponent* ACBossPawn::GetBlackBoard()
{
	UBlackboardComponent* BlackBoard = GetController<AAIController>()->GetBlackboardComponent();
	if (ensure(BlackBoard)) {
		return BlackBoard;
	}
	return nullptr;
}

