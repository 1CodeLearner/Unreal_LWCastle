// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTakenDamageDelegate, int, CurrentHealth, int, damage);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(const int Damage);
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	FTakenDamageDelegate OnTakenDamage;

protected:
	UFUNCTION(BlueprintCallable)
	int GetCurrency() const;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Attribute")
	int Currency;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Attribute")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxHealth;

};
