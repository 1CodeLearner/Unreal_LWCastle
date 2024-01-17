// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CAction.generated.h"

class UCGameplayComponent;
/**
 *
 */
UCLASS(Blueprintable)
class LWCASTLE_API UCAction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void Initialize( UCGameplayComponent* GameplayComp);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void InterruptAction(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanInterrupt(AActor* InstigatorActor, FGameplayTagContainer OtherGrantedTag) const;

	UFUNCTION(BlueprintCallable)
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable)
	FName GetActionName() const;

	UFUNCTION(BlueprintCallable)
	UCGameplayComponent* GetGameplayComponent() const;

	UFUNCTION(BlueprintCallable)
	float GetActionDuration() const;

protected:
	virtual UWorld* GetWorld() const override;

public:
	UCAction();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer GrantedTags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer BlockedTags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer InterruptedTags;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bCanInterrupt;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bHasDuration;

private:
	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	UCGameplayComponent* GameplayCompRef;

	UFUNCTION()
	void StopDuration(AActor* InstigatorActor);
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", AllowPrivateAccess = "true"), Category = "Action")
	float ActionDuration;
	FTimerHandle DurationHandle;
	FTimerDelegate DurationDelegate;
};
