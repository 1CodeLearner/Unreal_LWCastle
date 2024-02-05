// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionPause.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "CActionPause_ADS.generated.h"

class Auwol_test;
/**
 * 
 */
UCLASS()
class LWCASTLE_API UCActionPause_ADS : public UCActionPause
{
	GENERATED_BODY()
	virtual void Initialize_Implementation(UCGameplayComponent* Component) override;
	virtual void StartAction_Implementation(AActor* Instigator) override; 
	virtual void PauseAction_Implementation(AActor* Instigator) override;
	virtual void UnPauseAction_Implementation(AActor* Instigator) override;
	virtual void CompleteAction_Implementation(AActor* Instigator) override; 
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override; 

private: 
	UPROPERTY()
	Auwol_test* Character;
};
