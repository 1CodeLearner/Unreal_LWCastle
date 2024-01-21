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
	virtual void Initialize(UCGameplayComponent* GameplayComp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	bool CanStart(AActor* InstigatorActor) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool CanInterrupt(AActor* InstigatorActor, UCAction* OtherAction) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void InterruptAction(AActor* InstigatorActor);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void CompleteAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	FName GetActionName() const;

	UFUNCTION(BlueprintCallable)
	bool IsAutoStart() const;

	UFUNCTION(BlueprintCallable)
	UCGameplayComponent* GetGameplayComponent() const;

protected:
	virtual UWorld* GetWorld() const override;

public:
	UCAction();
	
	virtual FGameplayTagContainer GetGrantedTags() const;

	UFUNCTION(BlueprintCallable)
	virtual bool IsRunning() const;

protected:

	//Tags that will be granted to GameplayComponent for registering on-going Action
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer GrantedTags;
	//Stops action if same tag exists in GameplayComponent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer BlockedTags;
	//Stops on-going Action in GameplayComponent if tag exists in Action that is about to Start.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTagContainer InterruptedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bCanInterrupt;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;


private:
	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	UCGameplayComponent* GameplayCompRef;
};
