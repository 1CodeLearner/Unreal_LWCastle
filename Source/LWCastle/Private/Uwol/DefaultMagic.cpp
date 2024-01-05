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

	// �浹ü ���
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// �浹�������� ����
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	// �浹ü ũ�� ����
	collisionComp->SetSphereRadius(13);
	// ��Ʈ�� ���
	RootComponent = collisionComp;
	// �ܰ� ������Ʈ ���
	bodyMehsComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	// �θ� ������Ʈ ����
	bodyMehsComp->SetupAttachment(collisionComp);
	// �浹 ��Ȱ��ȭ
	bodyMehsComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// �ܰ�ũ�� ����
	bodyMehsComp->SetRelativeScale3D(FVector(1.0f));
	// �߻�ü ������Ʈ
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// movement ������Ʈ�� ���Ž�ų ������Ʈ ����
	movementComp->SetUpdatedComponent(collisionComp);
	// �ʱ� �ӵ�
	movementComp->InitialSpeed = 5000;
	// �ִ�ӵ�
	movementComp->MaxSpeed = 5000;
	// �ݵ�����
	movementComp->bShouldBounce = true;
	// �ݵ� ��
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

