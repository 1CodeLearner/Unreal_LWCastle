// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CAction.h"
#include "Justin/CActionPauseInterface.h"
#include "CActionPause.generated.h"

/**
 *
 */
UCLASS(Abstract)
class LWCASTLE_API UCActionPause : public UCAction, public ICActionPauseInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(Category = "ActionPause")
	virtual bool CanPause(AActor* InstigatorActor, UCAction* OtherAction) const override;

	UFUNCTION(Category = "ActionPause")
	void PauseAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(Category = "ActionPause")
	virtual bool CanUnPause(AActor* InstigatorActor, UCAction* OtherAction) const override;

	UFUNCTION(Category = "ActionPause")
	void UnPauseAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(Category = "ActionPause")
	virtual bool IsPausing() const override;
	
	virtual void InterruptAction_Implementation(AActor* InstigatorActor) override;
	virtual void CompleteAction_Implementation(AActor* InstigatorActor) override;

protected:
	//Pauses on-going action in GamplayComponent if same tag exists in starting Action's GrantedTags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer PausedTags;

	UPROPERTY(EditDefaultsOnly, Category = "PauseSetting")
	bool bCanPause;

public:
	UCActionPause();

private:
	bool bIsPausing;
};
