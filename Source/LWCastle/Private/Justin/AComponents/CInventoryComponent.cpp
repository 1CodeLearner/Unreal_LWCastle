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
}

int UCInventoryComponent::GetCurrency() const
{
	return Currency;
}

void UCInventoryComponent::AddCurrency(int AmountToAdd)
{
	Currency += AmountToAdd;
}

void UCInventoryComponent::SpendCurrency(int AmountToSpend)	
{
	Currency -= AmountToSpend;
	OnCurrencySpent.Broadcast(Currency);
}

bool UCInventoryComponent::HasEnoughCurrency(int AmountToSpend)
{
	return (Currency - AmountToSpend) >= 0;
}

