// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrencyValueChangeDelegate, int, CurrentCurrency);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCInventoryComponent();
	UPROPERTY(BLueprintAssignable, VisibleAnywhere, Category = "InventoryEvent | Currency")
	FCurrencyValueChangeDelegate OnCurrencyValueChange;

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	int GetCurrency() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	bool HasEnoughCurrency(int AmountToSpend) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	void SpendCurrency(int AmountToSpend);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	void AddCurrency(int AmountToAdd);

protected:
	virtual void BeginPlay() override;

	friend class UCPlayerAttributeManagerComp;

private:

	int Currency;
};
