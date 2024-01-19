// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "Action_Dodge.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UAction_Dodge : public UCAction
{
	GENERATED_BODY()
protected:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	TObjectPtr<UAnimMontage> AnimationMontage;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	float MontageRate = 1.f;

};
