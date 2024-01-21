// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FTakenDamageDelegate, AActor*, Instigator, UActorComponent*, OwnerComp, float, CurrentHealth, float, MaxHealth, float, damage);


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
	void ApplyDamage(AActor* Instigator, const int Damage);
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UPROPERTY(BlueprintAssignable)
	FTakenDamageDelegate OnTakenDamage;

protected:
	UFUNCTION(BlueprintCallable)
	int GetCurrency() const;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Attribute")
	int Currency;
	UPROPERTY(visibleAnywhere, Transient, BlueprintReadWrite, Category = "Attribute")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float MaxHealth;

};
