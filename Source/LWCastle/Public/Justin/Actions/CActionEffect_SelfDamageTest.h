// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionEffect.h"
#include "CActionEffect_SelfDamageTest.generated.h"

class UCAttributeComponent;
/**
 * 
 */
UCLASS()
class LWCASTLE_API UCActionEffect_SelfDamageTest : public UCActionEffect
{
	GENERATED_BODY()
public:

	virtual void IntervalStart(AActor* InstigatorActor) override;

	UPROPERTY(EditDefaultsOnly)
	float DamageAmount = 0;
};
