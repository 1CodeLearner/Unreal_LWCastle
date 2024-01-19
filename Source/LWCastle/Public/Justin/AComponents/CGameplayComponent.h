// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CGameplayComponent.generated.h"

class UCAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LWCASTLE_API UCGameplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<UCAction> NewActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StartActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopActionByName(AActor* InstigatorActor, FName ActionName);
	
	UPROPERTY(VisibleAnywhere, Category = "Action")
	FGameplayTagContainer ActiveGameplayTags;
public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	TArray<UCAction*> GetActions() const;
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TArray<TSubclassOf<UCAction>>  ActionClasses;

	UPROPERTY(VisibleAnywhere, Category = "Action")
	TArray<UCAction*> Actions;

public:
	UCGameplayComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
