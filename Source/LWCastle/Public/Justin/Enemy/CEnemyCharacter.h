// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEnemyCharacter.generated.h"

class UCGameplayComponent;
class UCEnemyAttributeComp;
UCLASS()
class LWCASTLE_API ACEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	void StartBoss();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Boss")
	bool bIsBossStage;


public:
	ACEnemyCharacter();
	TObjectPtr<UCGameplayComponent> GameplayComp;
	TObjectPtr<UCEnemyAttributeComp> AttributeComp;
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
