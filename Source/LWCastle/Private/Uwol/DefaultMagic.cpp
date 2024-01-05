// Fill out your copyright notice in the Description page of Project Settings.


#include "Uwol/DefaultMagic.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ADefaultMagic::ADefaultMagic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/InfinityBladeEffects/Enemy/Enemy_Chicken/Meshes/S_Survival_CA_Chicken_02.S_Survival_CA_Chicken_02'"));

	// 충돌체 등록
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 충돌프로파일 설정
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	// 충돌체 크기 설정
	collisionComp->SetSphereRadius(13);
	// 루트로 등록
	RootComponent = collisionComp;
	// 외관 컴포넌트 등록
	bodyMehsComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	// 부모 컴포넌트 지정
	bodyMehsComp->SetupAttachment(collisionComp);
	// 충돌 비활성화
	bodyMehsComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 외관크기 설정
	bodyMehsComp->SetRelativeScale3D(FVector(1.0f));
	// 발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// movement 컴포넌트가 갱신시킬 컴포넌트 지정
	movementComp->SetUpdatedComponent(collisionComp);
	// 초기 속도
	movementComp->InitialSpeed = 5000;
	// 최대속도
	movementComp->MaxSpeed = 5000;
	// 반동여부
	movementComp->bShouldBounce = true;
	// 반동 값
	movementComp->Bounciness = 0.3f;

}

// Called when the game starts or when spawned
void ADefaultMagic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

