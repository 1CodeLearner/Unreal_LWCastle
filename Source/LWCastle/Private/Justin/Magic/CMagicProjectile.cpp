// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagicProjectile.h"
#include "Components/BoxComponent.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/CGameplayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACMagicProjectile::ACMagicProjectile()
{
	BoxComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
}

void ACMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACMagicProjectile::OnOverlapBegin);
	BoxComp->OnComponentHit.AddDynamic(this, &ACMagicProjectile::OnHitBegin);
}

void ACMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);

	AudioComp = UGameplayStatics::SpawnSoundAtLocation(this, SpawnSound, GetActorLocation());
}

void ACMagicProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Ignore"))
	{
		return;
	}

	if (Cast<AActor>(GetInstigator()) == OtherActor)
		return;
	if (UCGameplayLibrary::ApplyDamage(GetInstigator(), OtherActor, GetDamage()))
	{
		auto Comp = OtherActor->GetComponentByClass<UCAttributeComponent>();
		if (Comp && Comp->IsAlive())
		{
			UE_LOG(LogTemp,Warning, TEXT("1111"));
			UCGameplayLibrary::ApplyStunOn(GetInstigator(), OtherActor, OwnedTag);
		}
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation());
		Destroy();
	}
}

void ACMagicProjectile::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Ignore"))
	{
		return;
	}
	UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation());
	Destroy();

}
