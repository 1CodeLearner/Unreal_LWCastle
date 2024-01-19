// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Test.generated.h"


class UCGameplayComponent;
class UCPlayerAttributeComp;
class UCCombatComponent;

UCLASS()
class LWCASTLE_API ATest : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test")
	TObjectPtr<UCPlayerAttributeComp> PlayerAttributeComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test")
	TObjectPtr<UCGameplayComponent> GameplayComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test")
	TObjectPtr<UCCombatComponent> CombatComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
