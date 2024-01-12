// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameModeBase.h"
#include "Justin/CItemBase.h"

UClass* ACGameModeBase::GetItemClassByName(FName ItemName)
{
	FString Msg;
	if (ensure(!ItemName.IsNone()))
	{
		if (ensure(ItemBaseMap.Contains(ItemName))) {
			return ItemBaseMap[ItemName]/*->GetClass()*/;
		}
		else
		{
			Msg = FString::Printf(TEXT("ItemMap in CGameModeBase does not contain Key, %s"), *ItemName.ToString());
		}
	}
	else
	{
		Msg = FString::Printf(TEXT("Name was not passed in! ItemName is Empty!"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Msg, true);
	return nullptr;
}

TArray<TSubclassOf<UCItemBase>> ACGameModeBase::GetItems()
{
	return ItemBaseClasses;
}

UDataTable* ACGameModeBase::GetCurrentLevelsTable()
{
	return DT_CurrentLevels;
}

UDataTable* ACGameModeBase::GetProgressionTableOf(EPlayerStat StatType)
{
	switch(StatType)
	{
	case EPlayerStat::HEALTH: {
		return DT_HealthProgression;
		break;
	}
	case EPlayerStat::MANA: {
		return DT_ManaProgression;
		break;
	}
	case EPlayerStat::STAMINA: {
		return DT_StaminaProgression;
		break;
	}
	}
	return nullptr;
}

TArray<FStruct_Progression> ACGameModeBase::GetProgressionOf(EPlayerStat StatType) const
{
	TArray<FStruct_Progression*> ProgRows;
	switch (StatType) 
	{
		case EPlayerStat::HEALTH:
		{
			DT_HealthProgression->GetAllRows<FStruct_Progression>("String", ProgRows);
			break;
		}
		case EPlayerStat::MANA:
		{
			DT_ManaProgression->GetAllRows<FStruct_Progression>("String", ProgRows);
			break;
		}
		case EPlayerStat::STAMINA:
		{
			DT_StaminaProgression->GetAllRows<FStruct_Progression>("String", ProgRows);
			break;
		}
	}


	TArray<FStruct_Progression> ProgressionArr;
	for (int i = 0; i < ProgRows.Num(); ++i)
	{
		FStruct_Progression ProgressionRow = { ProgRows[i]->Level, ProgRows[i]->Amount,ProgRows[i]->LevelupCost };

		ProgressionArr.Add(ProgressionRow);
	}

	return ProgressionArr;
}

TArray<FStruct_Level> ACGameModeBase::GetCurrentLevels() const
{
	FString Context = "StringContext";
	TArray< FStruct_Level*> StatsPtr;
	TArray< FStruct_Level> Stats;
	DT_CurrentLevels->GetAllRows<FStruct_Level>(Context, StatsPtr);
	for (FStruct_Level* stat : StatsPtr)
	{
		FStruct_Level Level = *stat;
		Stats.Add(Level);
	}
	return Stats;
}

void ACGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	for (auto ItemBaseClass : ItemBaseClasses) 
	{
		if (ItemBaseClass) 
		{
			//UCItemBase* tempItem = NewObject<UCItemBase>(this, ItemBaseClass);
			UCItemBase* tempItem = ItemBaseClass.GetDefaultObject();
			ItemBaseMap.Add(tempItem->TagName, tempItem->GetClass());
		}
	}
}

void ACGameModeBase::InitGameState()
{
	Super::InitGameState();


}


FName ACGameModeBase::GetStatName(EPlayerStat PlayerStatEnum)
{
	switch (PlayerStatEnum)
	{
	case EPlayerStat::HEALTH:
	{
		return FName("Health");
	}
	case EPlayerStat::MANA:
	{
		return FName("Mana");
	}
	case EPlayerStat::STAMINA:
	{
		return FName("Stamina");
	}
	default:
	{
		return FName();
	}
	}
}

FName ACGameModeBase::GetStatName(EPlayerStat PlayerStatEnum) const
{
	switch (PlayerStatEnum)
	{
	case EPlayerStat::HEALTH:
	{
		return FName("Health");
	}
	case EPlayerStat::MANA:
	{
		return FName("Mana");
	}
	case EPlayerStat::STAMINA:
	{
		return FName("Stamina");
	}
	default:
	{
		return FName();
	}
	}
}