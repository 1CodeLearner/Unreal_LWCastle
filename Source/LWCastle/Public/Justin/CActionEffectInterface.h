// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CActionEffectInterface.generated.h"

class UCAction;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCActionEffectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LWCASTLE_API ICActionEffectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION( Category = "ActionEffect")
	virtual bool CanPause(AActor* InstigatorActor, UCAction* OtherAction) const = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "ActionEffect")
	void PauseAction(AActor* InstigatorActor);

	UFUNCTION(Category = "ActionEffect")
	virtual bool CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "ActionEffect")
	void UnPauseAction(AActor* InstigatorActor);


	UFUNCTION(Category = "ActionEffect")
	virtual bool IsPausing() const = 0;
};
