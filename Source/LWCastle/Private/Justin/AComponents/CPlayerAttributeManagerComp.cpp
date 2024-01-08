// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "Justin/CPlayerController.h"

UCPlayerAttributeManagerComp::UCPlayerAttributeManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCPlayerAttributeManagerComp::BeginPlay()
{
	Super::BeginPlay();

	PlayerLevelMap.Add("Health", { "Health", 3});
	PlayerLevelMap.Add("Mana", { "Mana", 1});
	PlayerLevelMap.Add("Stamina", { "Stamina", 2});

	{
		TArray<FStruct_PlayerAttribute> HealthProgressionArr;
		TArray<FStruct_PlayerAttribute*> HealthProgRows;
		DT_PlayerHealthList->GetAllRows<FStruct_PlayerAttribute>("String", HealthProgRows);
		for (int i = 0; i < HealthProgRows.Num(); ++i)
		{
			FString RowName = FString::Printf(TEXT("Health_%d"), i);
			FStruct_PlayerAttribute HealthProgressionRow = { HealthProgRows[i]->Level, HealthProgRows[i]->Amount,HealthProgRows[i]->LevelupCost };

			HealthProgressionArr.Add(HealthProgressionRow);
		}
		FStatProgressConversion Holder = { HealthProgressionArr };
		PlayerProgressionMap.Add("Health", Holder);
	}
	{
		TArray<FStruct_PlayerAttribute> ManaProgressionArr;
		TArray<FStruct_PlayerAttribute*> ManaProgRows;
		DT_PlayerManaList->GetAllRows<FStruct_PlayerAttribute>("String", ManaProgRows);
		for (int i = 0; i < ManaProgRows.Num(); ++i)
		{
			FString RowName = FString::Printf(TEXT("Health_%d"), i);
			FStruct_PlayerAttribute ManaProgressionRow = { ManaProgRows[i]->Level, ManaProgRows[i]->Amount, ManaProgRows[i]->LevelupCost };

			ManaProgressionArr.Add(ManaProgressionRow);
		}

		FStatProgressConversion Holder = { ManaProgressionArr };
		PlayerProgressionMap.Add("Mana", Holder);
	}

	{

		TArray<FStruct_PlayerAttribute> StaminaProgressionArr;
		TArray<FStruct_PlayerAttribute*> StaminaProgRows;
		DT_PlayerStaminaList->GetAllRows < FStruct_PlayerAttribute>("String", StaminaProgRows);
		for (int i = 0; i < StaminaProgRows.Num(); ++i)
		{
			FString RowName = FString::Printf(TEXT("Health_%d"), i);
			FStruct_PlayerAttribute StaminaProgressionRow = { StaminaProgRows[i]->Level, StaminaProgRows[i]->Amount,StaminaProgRows[i]->LevelupCost };

			StaminaProgressionArr.Add(StaminaProgressionRow);
		}

		FStatProgressConversion Holder = { StaminaProgressionArr };
		PlayerProgressionMap.Add("Stamina", Holder);
	}
}

void UCPlayerAttributeManagerComp::TryUpdatePlayerStat(FStruct_PlayerLevel UpdatedLevel)
{
	int result = GetLevel(UpdatedLevel.StatName);
	if (ensureMsgf( (result != -1), TEXT( "StatName for updated level not valid!" ) ) ) 
	{
		OnStatUpdateFailed.Broadcast(EFailReason::NOSTATNAME);
		return;
	}
	else if (ensureMsgf((PlayerLevelMap[UpdatedLevel.StatName].Level + 1 == UpdatedLevel.Level), TEXT("Can only increment level by one!")) )
	{
		OnStatUpdateFailed.Broadcast(EFailReason::NOTINCREMENTING);
		return;
	}
	else if (IsMaxReached(UpdatedLevel.StatName))
	{
		OnStatUpdateFailed.Broadcast(EFailReason::MAXREACHED);
	}
	//else if (HasNoCurrency()) 
	//{

	//}
	else{
		PlayerLevelMap[UpdatedLevel.StatName] = UpdatedLevel;
		//switch () 
		//{
		//OnPlayerStatUpdated.Broadcast()
		//}
		//OnPlayerStatUpdated.Broadcast(EPlayerStat::);
	}
}

int UCPlayerAttributeManagerComp::GetLevel(FName StatName) const
{
	if (ensure(StatName != NAME_None))
	{
		FStruct_PlayerLevel StatLevel = PlayerLevelMap[StatName];
		return StatLevel.Level;
	}
	return -1;
}

int UCPlayerAttributeManagerComp::GetHealthLevel() const
{
	FStruct_PlayerLevel HealthLevel = PlayerLevelMap["Health"];
	return HealthLevel.Level;
}

int UCPlayerAttributeManagerComp::GetManaLevel() const
{
	FStruct_PlayerLevel HealthLevel = PlayerLevelMap["Mana"];
	return HealthLevel.Level;
}

int UCPlayerAttributeManagerComp::GetStaminaLevel() const
{
	FStruct_PlayerLevel HealthLevel = PlayerLevelMap["Stamina"];
	return HealthLevel.Level;
}

int UCPlayerAttributeManagerComp::GetHealthLevelupCost() const
{
	if (ensure(DT_PlayerHealthList))
	{
		FStruct_PlayerLevel HealthLevel = PlayerLevelMap["Health"];

		FString Level(FString::Printf(TEXT("Health_%d"), HealthLevel.Level - 1));

		const FString StringContext(TEXT("PlayerHealth String Context"));
		auto PlayerHealth = DT_PlayerHealthList->FindRow<FStruct_PlayerAttribute>(*Level, StringContext);

		return PlayerHealth->LevelupCost;
	}

	return -1;
}

int UCPlayerAttributeManagerComp::GetManaLevelupCost() const
{
	if (ensure(DT_PlayerManaList))
	{
		FStruct_PlayerLevel ManaLevel = PlayerLevelMap["Mana"];

		FString Level(FString::Printf(TEXT("Mana_%d"), ManaLevel.Level - 1));

		const FString StringContext(TEXT("PlayerMana String Context"));
		auto PlayerMana = DT_PlayerManaList->FindRow<FStruct_PlayerAttribute>(*Level, StringContext);

		return PlayerMana->LevelupCost;
	}
	return -1;
}

int UCPlayerAttributeManagerComp::GetStaminaLevelupCost() const
{
	if (ensure(DT_PlayerStaminaList))
	{
		FStruct_PlayerLevel StaminaLevel = PlayerLevelMap["Stamina"];

		FString Level(FString::Printf(TEXT("Stamina_%d"), StaminaLevel.Level - 1));

		const FString StringContext(TEXT("PlayerStamina String Context"));
		auto PlayerStamina = DT_PlayerStaminaList->FindRow<FStruct_PlayerAttribute>(*Level, StringContext);

		if (PlayerStamina) {
			return PlayerStamina->LevelupCost;
		}
	}
	return -1;
}

bool UCPlayerAttributeManagerComp::IsMaxReached(FName StatName)
{
	FStruct_PlayerAttribute CurrProgressionLevel = GetCurrentProgressionOf(StatName);
	FStruct_PlayerAttribute LastProgressionLevel = GetLastProgressionOf(StatName);
	return CurrProgressionLevel.Level != LastProgressionLevel.Level;
}

FStruct_PlayerAttribute UCPlayerAttributeManagerComp::GetCurrentProgressionOf(FName StatName)
{
	return PlayerProgressionMap[StatName].ProgressionHolder[GetCurrentProgressionIndex(StatName)];
}

FStruct_PlayerAttribute UCPlayerAttributeManagerComp::GetLastProgressionOf(FName StatName)
{
	return PlayerProgressionMap[StatName].ProgressionHolder.Last();
}

int UCPlayerAttributeManagerComp::GetCurrentProgressionIndex(FName StatName)
{
	int CurrentLevel = PlayerLevelMap[StatName].Level;

	return CurrentLevel - 1;
}
