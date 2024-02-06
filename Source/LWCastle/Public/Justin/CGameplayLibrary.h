// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CGameplayLibrary.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UCGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameplayLibrary")
	static bool ApplyDamage(AActor* Invoker, AActor* AppliedActor, float Damage);

	static bool ApplyStunOn(AActor* Invoker, AActor* AppliedActor, FGameplayTagContainer StunTag);

private:
};
