// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameModeBase.h"
#include "Justin/CItemBase.h"

void ACGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	for (auto ItemBaseClass : ItemBaseClasses) 
	{
		if (ItemBaseClass) 
		{
			UCItemBase* tempItem = NewObject<UCItemBase>(this, ItemBaseClass);
			ItemBaseMap[tempItem->TagName] = ItemBaseClass;
		}
	}
}
