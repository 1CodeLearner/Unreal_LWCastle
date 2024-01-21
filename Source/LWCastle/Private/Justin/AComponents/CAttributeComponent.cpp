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
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float UCAttributeComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UCAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UCAttributeComponent::ApplyDamage(AActor* Instigator, const int Damage)
{
	CurrentHealth -= Damage;
	OnTakenDamage.Broadcast(Instigator, this, CurrentHealth, MaxHealth, Damage);
	if(IsAlive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Taken this mouch Damgae %d"), Damage);
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

