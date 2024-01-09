// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameMode.h"
#include "Justin/ItemBase.h"

ACGameMode::ACGameMode()
{

}

//UClass* ACGameMode::GetItemWithName(FName ItemName)
//{
//	return UClass();
//}

void ACGameMode::StartPlay()
{
	Super::StartPlay();

	for (auto ItemClass : ItemClasses) 
	{
		if (ItemClass) 
		{
			auto* DefaultObject = ItemClass.GetDefaultObject();
			FName Name = DefaultObject->ItemName;
			ItemMap[Name] = DefaultObject;
		}
	}
	
}
