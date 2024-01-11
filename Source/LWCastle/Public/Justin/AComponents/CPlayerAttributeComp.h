// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/AComponents/CAttributeComponent.h"
#include "Justin/PlayerStatTypes.h"
#include "CPlayerAttributeComp.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDepletedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManaDepletedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProgressonChangedDelegate, EPlayerStat, StatTypeEnum, float, MaxAmount);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCPlayerAttributeComp : public UCAttributeComponent
{
	GENERATED_BODY()

public:
	UCPlayerAttributeComp();
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FStaminaDepletedDelegate OnStaminaDepleted;
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FManaDepletedDelegate OnManaDepleted;
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FProgressonChangedDelegate OnProgressionChanged;
protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int GetCurrentMana() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina() const;

	UFUNCTION(BlueprintCallable)
	bool TrySpendMana(int SpendAmount);

	UFUNCTION(BlueprintCallable)
	void SpendStamina(float SpendAmount);

	UFUNCTION(BlueprintCallable)
	void EnableSpendingStaminaByRate(bool bIsEnabled);

protected:

	UFUNCTION()
	void OnStatUpdated(FStatInfo StatInfo);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAttribute")
	float MaxMana;
	float CurrentMana;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAttribute")
	float MaxStamina;
	float CurrentStamina;
private:
	UPROPERTY(VisibleDefaultsOnly)
	float StaminaSpendRate;
	FTimerHandle ManaRecoveryCooldownHandle;
	FTimerHandle StaminaRecoveryCooldownHandle;
};
