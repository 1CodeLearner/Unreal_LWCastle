// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 100;
	Currency = 50;
}

void UCAttributeComponent::BeginPlay()
{
	CurrentHealth = MaxHealth;
}

int UCAttributeComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

int UCAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UCAttributeComponent::ApplyDamage(const int Damage)
{
	CurrentHealth -= Damage;
	if(!IsAlive())
	{
		OnTakenDamage.Broadcast(CurrentHealth, Damage);
	}
}

bool UCAttributeComponent::IsAlive() const
{
	return CurrentHealth > 0;
}

int UCAttributeComponent::GetCurrency() const
{
	return Currency;
}

