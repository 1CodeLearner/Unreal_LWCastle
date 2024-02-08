// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "EnemyCharacter.generated.h"

class UCAttributeComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeSuccessDelegate);

UCLASS()
class LWCASTLE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Enemy")
	FMeleeSuccessDelegate OnMeleeSuccess;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UBoxComponent* MeleeBoxComp;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float HardMeleeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float LightMeleeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	FGameplayTagContainer OwnedTag;

public:	
	virtual void Tick(float DeltaTime) override;

};
