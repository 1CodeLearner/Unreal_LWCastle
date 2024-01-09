// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CInventoryComponent.h"

UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Currency = -1;
}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Currency = 30;
}

int UCInventoryComponent::GetCurrency() const
{
	return Currency;
}

void UCInventoryComponent::AddCurrency(int AmountToAdd)
{
	Currency += AmountToAdd;
}

bool UCInventoryComponent::TrySpendCurrency(int AmountToSpend)
{
	if (Currency - AmountToSpend < 0) {
		return false;
	}

	Currency -= AmountToSpend;
	OnCurrencySpent.Broadcast(Currency);
	return true;
}

