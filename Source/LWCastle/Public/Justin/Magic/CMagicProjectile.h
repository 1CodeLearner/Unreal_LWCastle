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
	ACMagicProjectile();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FGameplayTagContainer OwnedTag;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* SpawnSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ImpactSound;
private:
	UPROPERTY()
	USoundBase* ActiveFlightSound;
	
};
