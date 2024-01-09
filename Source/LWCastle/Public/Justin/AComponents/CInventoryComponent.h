// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrencySpentDelegate, int, CurrencyRemaining);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LWCASTLE_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();
	UPROPERTY(BLueprintAssignable, VisibleAnywhere, Category = "InventoryEvent | Currency")
	FCurrencySpentDelegate OnCurrencySpent;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	int GetCurrency() const; 

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	void AddCurrency(int AmountToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Currency")
	bool TrySpendCurrency(int AmountToSpend);

private:
	int Currency;
};
