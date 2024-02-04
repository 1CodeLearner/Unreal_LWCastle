// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/CGameplayLibrary.h"

void ACMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACMagicProjectile::OnOverlapBegin);
	SphereComp->OnComponentHit.AddDynamic(this, &ACMagicProjectile::OnHitBegin);
}

void ACMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);
}

void ACMagicProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AActor>(GetInstigator()) == OtherActor)
		return;
	if (UCGameplayLibrary::ApplyDamage(GetInstigator(), OtherActor, GetDamage()))
	{
		Destroy();
	}
}

void ACMagicProjectile::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}
