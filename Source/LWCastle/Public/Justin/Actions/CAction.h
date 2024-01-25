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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Initialize(UCGameplayComponent* GameplayComp);

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

	//Tags that will be granted to GameplayComponent from this Action
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer GrantedTags;
	//Stops Action from starting if tag already exists in GameplayComponent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	/*Stops on-going Action in GameplayComponent if the same Tag exists in starting Action's GrantedTags.
	Takes first priority over paused Tags*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer InterruptedTags;

	UPROPERTY(EditDefaultsOnly, Category = "ActionSettings")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "ActionSettings")
	bool bCanInterrupt;


private:
	UPROPERTY()
	bool bIsRunning;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"),Category = "ActionSettings")
	bool bAutoStart;
	UPROPERTY()
	UCGameplayComponent* GameplayCompRef;
};
