// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/CGameplayLibrary.h"
#include "Justin/AComponents/CGameplayComponent.h"

ACMagicProjectile::ACMagicProjectile()
{
	SphereComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

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
		auto OtherGameplayComp = OtherActor->GetComponentByClass<UCGameplayComponent>();
		
		if (OtherGameplayComp)
		{
			FVector VectorDir = GetInstigator()->GetActorLocation() - OtherActor->GetActorLocation() ;

			VectorDir.Normalize();
			FRotator Rotation = VectorDir.Rotation();
			Rotation.Roll = 0.f;

			OtherActor->SetActorRotation(Rotation);

			if (OwnedTag.HasTagExact(FGameplayTag::RequestGameplayTag("State.Stun.Hard")))
			{
				//OtherGameplayComp->StartActionByName(GetInstigator(), "Stunhard");
			}
			else if (OwnedTag.HasTagExact(FGameplayTag::RequestGameplayTag("State.Stun.Light")))
			{
				OtherGameplayComp->StartActionByName(GetInstigator(), "StunLight");
			}
		}
		Destroy();
	}
}

void ACMagicProjectile::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}
