// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/UMBaseProjectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AUMBaseProjectile::AUMBaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	RootComponent = BoxComp;

	BoxComp->SetCollisionObjectType(ECC_GameTraceChannel3);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);

	ParticleSysComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSysComp");
	ParticleSysComp->SetupAttachment(RootComponent);

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	ProjectileMoveComp->bRotationFollowsVelocity = true;
	ProjectileMoveComp->bInitialVelocityInLocalSpace = true;
	ProjectileMoveComp->ProjectileGravityScale = 0.f;
}
float AUMBaseProjectile::GetProjectileDistance() const
{
	return ProjectileMaxDistance;
}
void AUMBaseProjectile::SetProjectileSpeed(float NewProjectileSpeed)
{
	ProjectileSpeed = NewProjectileSpeed;
}


void AUMBaseProjectile::PreInitializeComponents()
{
	Super::PostInitializeComponents();
	APawn* InstigatorPawn = GetInstigator();
	if (InstigatorPawn)
	{
		BoxComp->IgnoreActorWhenMoving(InstigatorPawn, true);
		if (!InstigatorPawn->IsMoveInputIgnored())
		{
			InstigatorPawn->MoveIgnoreActorAdd(this);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Make sure to assign an Instigator to Projectile when Spawning it."));
	}
	if (ProjectileMoveComp)
	{
		ProjectileMoveComp->InitialSpeed = ProjectileSpeed;
	}
}

void AUMBaseProjectile::PostInitializeComponents()
{
	//When is this used?

}

float AUMBaseProjectile::GetDamage()
{
	return Damage;
}




