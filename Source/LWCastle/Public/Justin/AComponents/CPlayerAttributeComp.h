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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangeDelegate, EPlayerStat, StatTypeEnum, float, Current, float, MaxAmount);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCPlayerAttributeComp : public UCAttributeComponent
{
	GENERATED_BODY()

public:
	UCPlayerAttributeComp();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Attribute")
	FAttributeChangeDelegate OnAttributeChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Attribute")
	FStaminaDepletedDelegate OnStaminaDepleted;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Attribute")
	FManaDepletedDelegate OnManaDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FProgressonChangedDelegate OnProgressionChanged;

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FStruct_StatDisplays GetAttributesToDisplay() const;

	UFUNCTION(BlueprintCallable)
	void RecoverToFull();

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentMana() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable)
	bool TryChannelMana(float Value);
	UFUNCTION(BlueprintCallable)
	void CancelChannelingMana();
	UFUNCTION(BlueprintCallable)
	void CompleteChannelingMana();

	UFUNCTION(BlueprintCallable)
	bool TrySpendMana(float SpendAmount);
	UFUNCTION(BlueprintCallable)
	bool SpendStamina(float SpendAmount);

protected:

	UFUNCTION(BlueprintCallable)
	void EnableSpendingStaminaByRate(bool bIsEnabled);

protected:

	UFUNCTION()
	void OnStatUpdated(FStatInfo StatInfo);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAttribute")
	float MaxMana;
	float CurrentMana;

	float ChanneledManaAmount;
	bool bIsChannelingMana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAttribute")
	float MaxStamina;
	float CurrentStamina;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1.0"), Category = "PlayerAttribute")
	float StaminaRecoveryRate;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1.0"), Category = "PlayerAttribute")
	float ManaRecoveryRate;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.5"), Category = "PlayerAttribute")
	float StaminaRecoveryDelay;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.5"), Category = "PlayerAttribute")
	float ManaRecoveryDelay;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.5"), Category = "PlayerAttribute")
	float ChannelManaRecoveryDelay;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.5"), Category = "PlayerAttribute")
	float ChannelManaDepletedDelay;

private:

	void DisplayStats(EPlayerStat StatType);

	void StartManaRecoveryCooldown();

	FTimerHandle ManaRecoveryCooldownHandle;
	FTimerHandle StaminaRecoveryCooldownHandle;
	FTimerHandle ChannelManaRecoveryHandle;
	FTimerHandle ChannelManaDepletedHandle;

	FTimerDelegate ManaRecoveryDelegate;
	FTimerDelegate StaminaRecoveryDelegate;
	FTimerDelegate ChannelManaRecoveryDelegate;

	void CheckAndDisableTick();

	UFUNCTION()
	void EnableManaRecovery(bool bEnabled);
	UFUNCTION()
	void EnableStaminaRecovery(bool bEnabled);
	UFUNCTION()
	void EnableChannelManaRecovery(bool bEnabled);
	UFUNCTION()
	void ChannelManaDepletedReset();

	bool bIsManaRecovering;
	bool bIsStaminaRecovering;
	bool bIsChannelManaRecovering;
	bool bIsChannelingManaDepleted;
};
