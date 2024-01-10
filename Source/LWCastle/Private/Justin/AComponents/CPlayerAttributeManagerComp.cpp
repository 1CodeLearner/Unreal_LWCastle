// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "Justin/CPlayerController.h"
#include "Justin/AComponents/CInventoryComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Justin/CGameModeBase.h"

UCPlayerAttributeManagerComp::UCPlayerAttributeManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCPlayerAttributeManagerComp::BeginPlay()
{
	Super::BeginPlay();

	PlayerLevelMap.Add("Health", { "Health", 3 });
	PlayerLevelMap.Add("Mana", { "Mana", 1 });
	PlayerLevelMap.Add("Stamina", { "Stamina", 2 });

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	ACGameModeBase* CGameMode = Cast<ACGameModeBase>(GameMode);
	if (CGameMode) 
	{

	}
	{
		TArray<FStruct_PlayerAttribute> HealthProgressionArr;
		TArray<FStruct_PlayerAttribute*> HealthProgRows;
		DT_HealthProgression->GetAllRows<FStruct_PlayerAttribute>("String", HealthProgRows);
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
		DT_ManaProgression->GetAllRows<FStruct_PlayerAttribute>("String", ManaProgRows);
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
		DT_StaminaProgression->GetAllRows < FStruct_PlayerAttribute>("String", StaminaProgRows);
		for (int i = 0; i < StaminaProgRows.Num(); ++i)
		{
			FString RowName = FString::Printf(TEXT("Health_%d"), i);
			FStruct_PlayerAttribute StaminaProgressionRow = { StaminaProgRows[i]->Level, StaminaProgRows[i]->Amount,StaminaProgRows[i]->LevelupCost };

			StaminaProgressionArr.Add(StaminaProgressionRow);
		}

		FStatProgressConversion Holder = { StaminaProgressionArr };
		PlayerProgressionMap.Add("Stamina", Holder);
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

	InventoryComp->SpendCurrency(GetLevelupCostFor(GetStatName(PlayerStatType))	);
	IncrementStatLevel(GetStatName(PlayerStatType));
	FStruct_PlayerAttribute	UpdatedStat = GetCurrentProgressionOf(GetStatName(PlayerStatType));
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
		FStruct_PlayerAttribute CurrStat = GetCurrentProgressionOf(GetStatName(StatType));
		return { StatType, CurrStat.Level, CurrStat.LevelupCost, CheckIsMaxFor(GetStatName(StatType)) };
	}
	return FStatInfo();
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

bool UCPlayerAttributeManagerComp::IsMaxReached(FName StatName)
{
	FStruct_PlayerAttribute CurrProgressionLevel = GetCurrentProgressionOf(StatName);
	FStruct_PlayerAttribute LastProgressionLevel = GetLastProgressionOf(StatName);
	return CurrProgressionLevel.Level == LastProgressionLevel.Level;
}

FStruct_PlayerAttribute UCPlayerAttributeManagerComp::GetCurrentProgressionOf(FName StatName)
{
	return PlayerProgressionMap[StatName].ProgressionHolder[GetCurrentProgressionIndex(StatName)];
}

FStruct_PlayerAttribute UCPlayerAttributeManagerComp::GetCurrentProgressionOf(FName StatName) const
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

int UCPlayerAttributeManagerComp::GetCurrentProgressionIndex(FName StatName) const
{
	int CurrentLevel = PlayerLevelMap[StatName].Level;

	return CurrentLevel - 1;
}

int UCPlayerAttributeManagerComp::GetLevelupCostFor(FName StatName)
{
	FStruct_PlayerAttribute PlayerProgression = GetCurrentProgressionOf(StatName);
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

void UCPlayerAttributeManagerComp::IncrementStatLevel(FName StatName)
{
	FStruct_PlayerLevel CurrPlayerLevel = PlayerLevelMap[StatName];
	PlayerLevelMap[StatName].Level = CurrPlayerLevel.Level + 1;
}
