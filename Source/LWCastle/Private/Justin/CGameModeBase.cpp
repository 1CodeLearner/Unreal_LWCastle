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
