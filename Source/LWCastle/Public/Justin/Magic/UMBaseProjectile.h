// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UMBaseProjectile.generated.h"

UCLASS()
class LWCASTLE_API AUMBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUMBaseProjectile();
	float GetProjectileDistance() const;
	void SetProjectileSpeed(float NewProjectileSpeed);
protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	float GetDamage();
protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* SphereComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystemComponent* ParticleSysComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMoveComp;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ProjectileSpeed = 2000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ProjectileMaxDistance = 2500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Damage = -20.f;
};
