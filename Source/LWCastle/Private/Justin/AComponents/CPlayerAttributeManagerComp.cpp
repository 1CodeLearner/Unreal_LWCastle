// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "Justin/CPlayerController.h"
#include "Justin/AComponents/CInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/CGameModeBase.h"

UCPlayerAttributeManagerComp::UCPlayerAttributeManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCPlayerAttributeManagerComp::BeginPlay()
{
	Super::BeginPlay();

	//Bring Player progression info from GameMode.
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	ACGameModeBase* CGameMode = Cast<ACGameModeBase>(GameMode);
	if (CGameMode)
	{
		{
			TArray<FStruct_Progression> Progession = CGameMode->GetProgressionOf(EPlayerStat::HEALTH);
			FStatProgressConversion Holder = { Progession };
			PlayerProgressionMap.Add("Health", Holder);
		}
		{
			TArray<FStruct_Progression> Progession = CGameMode->GetProgressionOf(EPlayerStat::MANA);
			FStatProgressConversion Holder = { Progession };
			PlayerProgressionMap.Add("Mana", Holder);
		}
		{
			TArray<FStruct_Progression> Progession = CGameMode->GetProgressionOf(EPlayerStat::STAMINA);
			FStatProgressConversion Holder = { Progession };
			PlayerProgressionMap.Add("Stamina", Holder);
		}
		{

			TArray<FStruct_Level> LevelStats = CGameMode->GetCurrentLevels();
			for (FStruct_Level stat : LevelStats)
			{
				PlayerLevelMap.Add(stat.StatName, stat);
			}
		}
	}

	InventoryComp = Cast<APlayerController>(GetOwner())->GetPawn()->GetComponentByClass<UCInventoryComponent>();
	if (ensureAlwaysMsgf(InventoryComp, TEXT("Inventory Missing! Add Inventory Component to controlled Character!"))) {
		UE_LOG(LogTemp, Warning, TEXT("SUCCESS"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FAIL"));
	}
}

void UCPlayerAttributeManagerComp::UpdatePlayerStat(EPlayerStat PlayerStatType)
{
	//Not enough currency
	if (!ensure(InventoryComp->HasEnoughCurrency(GetLevelupCostFor(GetStatName(PlayerStatType)))))
	{
		return;
	}
	//PlayerStatType is NONE
	if (!ensure(PlayerStatType != EPlayerStat::NONE)) {
		return;
	}

	//Is Max Level
	if (!ensure(!CheckIsMaxFor(GetStatName(PlayerStatType))))
	{
		return;
	}

	InventoryComp->SpendCurrency(GetLevelupCostFor(GetStatName(PlayerStatType)));
	IncrementStatLevel(GetStatName(PlayerStatType));
	FStruct_Progression	UpdatedStat = GetCurrentProgressionOf(GetStatName(PlayerStatType));
	OnPlayerStatUpdated.Broadcast(
		{
			PlayerStatType,
			UpdatedStat.Level,
			UpdatedStat.LevelupCost,
			CheckIsMaxFor(GetStatName(PlayerStatType))
		}
	);
}

FStatInfo UCPlayerAttributeManagerComp::GetStatInfo(EPlayerStat StatType) const
{
	if (ensure(StatType != EPlayerStat::NONE))
	{
		FStruct_Progression CurrStat = GetCurrentProgressionOf(GetStatName(StatType));
		return { StatType, CurrStat.Level, CurrStat.LevelupCost, CheckIsMaxFor(GetStatName(StatType)) };
	}
	return FStatInfo();
}

int UCPlayerAttributeManagerComp::GetHealthLevel() const
{
	FStruct_Level HealthLevel = PlayerLevelMap["Health"];
	return HealthLevel.Level;
}

int UCPlayerAttributeManagerComp::GetManaLevel() const
{
	FStruct_Level HealthLevel = PlayerLevelMap["Mana"];
	return HealthLevel.Level;
}

int UCPlayerAttributeManagerComp::GetStaminaLevel() const
{
	FStruct_Level HealthLevel = PlayerLevelMap["Stamina"];
	return HealthLevel.Level;
}

bool UCPlayerAttributeManagerComp::IsMaxReached(FName StatName)
{
	FStruct_Progression CurrProgressionLevel = GetCurrentProgressionOf(StatName);
	FStruct_Progression LastProgressionLevel = GetLastProgressionOf(StatName);
	return CurrProgressionLevel.Level == LastProgressionLevel.Level;
}

FStruct_Progression UCPlayerAttributeManagerComp::GetCurrentProgressionOf(FName StatName)
{
	return PlayerProgressionMap[StatName].ProgressionHolder[GetCurrentProgressionIndex(StatName)];
}

FStruct_Progression UCPlayerAttributeManagerComp::GetCurrentProgressionOf(FName StatName) const
{
	return PlayerProgressionMap[StatName].ProgressionHolder[GetCurrentProgressionIndex(StatName)];
}

FStruct_Progression UCPlayerAttributeManagerComp::GetLastProgressionOf(FName StatName)
{
	return PlayerProgressionMap[StatName].ProgressionHolder.Last();
}

int UCPlayerAttributeManagerComp::GetCurrentProgressionIndex(FName StatName)
{
	int CurrentLevel = PlayerLevelMap[StatName].Level;

	return CurrentLevel - 1;
}

int UCPlayerAttributeManagerComp::GetCurrentProgressionIndex(FName StatName) const
{
	int CurrentLevel = PlayerLevelMap[StatName].Level;

	return CurrentLevel - 1;
}

int UCPlayerAttributeManagerComp::GetLevelupCostFor(FName StatName)
{
	FStruct_Progression PlayerProgression = GetCurrentProgressionOf(StatName);
	return PlayerProgression.LevelupCost;
}

bool UCPlayerAttributeManagerComp::CheckIsMaxFor(FName StatName)
{
	return GetCurrentProgressionOf(StatName).LevelupCost == 0;
}

bool UCPlayerAttributeManagerComp::CheckIsMaxFor(FName StatName) const
{
	return GetCurrentProgressionOf(StatName).LevelupCost == 0;
}




void UCPlayerAttributeManagerComp::IncrementStatLevel(FName StatName)
{
	FStruct_Level CurrPlayerLevel = PlayerLevelMap[StatName];
	PlayerLevelMap[StatName].Level = CurrPlayerLevel.Level + 1;
}



FName UCPlayerAttributeManagerComp::GetStatName(EPlayerStat PlayerStatEnum)
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

FName UCPlayerAttributeManagerComp::GetStatName(EPlayerStat PlayerStatEnum) const
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