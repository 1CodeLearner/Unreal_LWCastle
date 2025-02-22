// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CActionPauseInterface.generated.h"

class UCAction;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCActionPauseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LWCASTLE_API ICActionPauseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool CanPause(AActor* InstigatorActor, UCAction* OtherAction) const = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "ActionEffect")
	void PauseAction(AActor* InstigatorActor);

	virtual bool CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "ActionEffect")
	void UnPauseAction(AActor* InstigatorActor);

	virtual bool IsPausing() const = 0;
};
