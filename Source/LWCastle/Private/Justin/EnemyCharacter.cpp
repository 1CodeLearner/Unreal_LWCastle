// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/EnemyCharacter.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Justin/CGameplayLibrary.h"
#include "Justin/AComponents/CGameplayComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");
	MeleeBoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComponent_Attack");
	MeleeBoxComp->SetupAttachment(RootComponent);
	MeleeBoxComp->SetBoxExtent(FVector(90.5f, 80.f, 60.f));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MeleeBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlap);
}

void AEnemyCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != Cast<AActor>(this))
	{
		if (ensure(OnMeleeSuccess.IsBound()))
		{
			if (UCGameplayLibrary::ApplyDamage(this, OtherActor, -MeleeDamage))
			{
				UCGameplayLibrary::ApplyStunOn(this, OtherActor, OwnedTag);
				OnMeleeSuccess.Broadcast();
			}
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

