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

		FPlayerLevel* HealthLevel = DT_PlayerLevels->FindRow<FPlayerLevel>("Health", StringContext);

		FString Level(FString::Printf(TEXT("Health_%d"), HealthLevel->Level - 1));

		auto PlayerHealth = DT_PlayerHealthList->FindRow<FStruct_PlayerAttribute>(*Level, StringContext);

		return PlayerHealth->LevelupCost;
	}

	return -1;
}

int UCPlayerAttributeManagerComp::GetManaLevelupCost() const
{
	if (ensure(DT_PlayerManaList))
	{
		const FString StringContext(TEXT("PlayerMana String Context"));

		FPlayerLevel* ManaLevel = DT_PlayerLevels->FindRow<FPlayerLevel>("Mana", StringContext);

		FString Level(FString::Printf(TEXT("Mana_%d"), ManaLevel->Level - 1));

		auto PlayerMana = DT_PlayerManaList->FindRow<FStruct_PlayerAttribute>(*Level, StringContext);

		return PlayerMana->LevelupCost;
	}
	return -1;
}

int UCPlayerAttributeManagerComp::GetStaminaLevelupCost() const
{
	if (ensure(DT_PlayerStaminaList))
	{
		const FString StringContext(TEXT("PlayerStamina String Context"));

		FPlayerLevel* StaminaLevel = DT_PlayerLevels->FindRow<FPlayerLevel>("Stamina", StringContext);

		FString Level(FString::Printf(TEXT("Stamina_%d"), StaminaLevel->Level - 1));

		auto PlayerStamina = DT_PlayerStaminaList->FindRow<FStruct_PlayerAttribute>(*Level, StringContext);

		return PlayerStamina->LevelupCost;
	}
	return -1;
}
