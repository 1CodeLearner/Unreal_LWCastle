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
	Currency = 120;
	OnCurrencyValueChange.Broadcast(Currency);
}

int UCInventoryComponent::GetCurrency() const
{
	return Currency;
}

void UCInventoryComponent::AddCurrency(int AmountToAdd)
{
	Currency += AmountToAdd;
	OnCurrencyValueChange.Broadcast(GetCurrency());
}

void UCInventoryComponent::SpendCurrency(int AmountToSpend)	
{
	Currency -= AmountToSpend;
	OnCurrencyValueChange.Broadcast(GetCurrency());
}

bool UCInventoryComponent::HasEnoughCurrency(int AmountToSpend) const
{
	return (Currency - AmountToSpend) >= 0;
}

