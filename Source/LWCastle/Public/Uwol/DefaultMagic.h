// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultMagic.generated.h"

UCLASS()
class LWCASTLE_API ADefaultMagic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultMagic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// magic movement
	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent* movementComp;

	// Collision Component
	UPROPERTY(VisibleAnywhere, Category=Collision)
	class USphereComponent* collisionComp;

	// Mesh Component
	UPROPERTY(VisibleAnywhere, Category=BodyMesh)
	class UStaticMeshComponent* bodyMehsComp;

	// ÃÑ¾ËÁ¦°Å
	void Die();

};
