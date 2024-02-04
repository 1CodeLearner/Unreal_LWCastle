// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Magic/UMBaseProjectile.h"
#include "GameplayTagContainer.h"
#include "CMagicProjectile.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API ACMagicProjectile : public AUMBaseProjectile
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FGameplayTagContainer OwnedTag;

};
