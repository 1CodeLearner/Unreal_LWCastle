// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeManagerComp.h"


UCPlayerAttributeManagerComp::UCPlayerAttributeManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;

}

int UCPlayerAttributeManagerComp::GetHealthLevel() const
{
	if (ensure(DT_PlayerLevels))
	{
		const FString StringContext(TEXT("PlayerLevel String Context"));
		FPlayerLevel* HealthLevel = DT_PlayerLevels->FindRow<FPlayerLevel>(FName(TEXT("Health")), StringContext);
		return HealthLevel->Level;
	}
	return -1;
}

int UCPlayerAttributeManagerComp::GetManaLevel() const
{
	if (ensure(DT_PlayerLevels))
	{
		const FString StringContext(TEXT("PlayerLevel String Context"));
		FPlayerLevel* ManaLevel = DT_PlayerLevels->FindRow<FPlayerLevel>(FName(TEXT("Mana")), StringContext);
		return ManaLevel->Level;
	}

	return -1;
}

int UCPlayerAttributeManagerComp::GetStaminaLevel() const
{
	if (ensure(DT_PlayerLevels))
	{
		const FString StringContext(TEXT("PlayerLevel String Context"));
		FPlayerLevel* StaminaLevel = DT_PlayerLevels->FindRow<FPlayerLevel>(FName(TEXT("Stamina")), StringContext);
		return StaminaLevel->Level;
	}

	return -1;
}

int UCPlayerAttributeManagerComp::GetHealthLevelupCost() const
{
	if (ensure(DT_PlayerHealthList))
	{
		const FString StringContext(TEXT("PlayerHealth String Context"));
		/*
		FPlayerLevel* HealthLevel = DT_PlayerLevels->FindRow<FPlayerLevel>()
		auto PlayerHealth =  DT_PlayerHealthList->FindRow<>("")*/
		return 0;
	}

	return -1;
}

int UCPlayerAttributeManagerComp::GetManaLevelupCost() const
{
	if (ensure(DT_PlayerManaList))
	{
		const FString StringContext(TEXT("PlayerMana String Context"));
		
		return 0;
	}
	return -1;
}

int UCPlayerAttributeManagerComp::GetStaminaLevelupCost() const
{
	if (ensure(DT_PlayerStaminaList))
	{
		const FString StringContext(TEXT("PlayerStamina String Context"));
		
		return 0;
	}
	return -1;
}
