// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameModeBase.h"
#include "Justin/CItemBase.h"
#include "Justin/CGameplayLibrary.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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

TArray<FStruct_Progression> ACGameModeBase::GetProgressions(EPlayerStat StatType) const
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

FStruct_Progression ACGameModeBase::GetCurrentProgressionOf(EPlayerStat StatType, int Level)
{
	TArray<FStruct_Progression> temp = GetProgressions(StatType);
	ensureAlways(Level <= temp.Num() && Level > 0);
	return temp[Level - 1];
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


void ACGameModeBase::RestoreTime(AActor* ActorContext)
{
	UGameplayStatics::SetGlobalTimeDilation(ActorContext, 1.f);

	UCGameplayLibrary::AddCurrency(ActorContext);
}

/*void ACGameModeBase::RespawnPlayer(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}*/

void ACGameModeBase::SlowDownTime(AActor* ActorContext)
{
	UGameplayStatics::SetGlobalTimeDilation(ActorContext, .1f);
	FTimerDelegate Delegate;
	FTimerHandle Handle;
	Delegate.BindUFunction(this, "RestoreTime", ActorContext);
	GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, .05f, false);
	UGameplayStatics::PlaySound2D(this, DodgeSound);
}
