// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "CPlayerAttributeComp.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDepletedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManaDepletedDelegate);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LWCASTLE_API UCPlayerAttributeComp : public UCAttributeComponent
{
	GENERATED_BODY()

public:
	UCPlayerAttributeComp();
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FStaminaDepletedDelegate OnStaminaDepleted;
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FManaDepletedDelegate OnManaDepleted;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int GetManaPoints() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	UFUNCTION(BlueprintCallable)
	bool TrySpendMana(int SpendAmount);

	UFUNCTION(BlueprintCallable)
	void SpendStamina(float SpendAmount);

	UFUNCTION(BlueprintCallable)
	void EnableSpendingStaminaByRate(bool bIsEnabled);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAttribute")
	int ManaPoints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAttribute")
	float Stamina;
private:
	UPROPERTY(VisibleDefaultsOnly)
	float StaminaSpendRate;
	FTimerHandle ManaRecoveryCooldownHandle;
	FTimerHandle StaminaRecoveryCooldownHandle;
};
