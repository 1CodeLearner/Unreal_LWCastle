// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatTypes.generated.h"


UENUM(BlueprintType)
enum class EPlayerStat : uint8
{
	NONE,
	HEALTH,
	MANA,
	STAMINA
};

USTRUCT(BlueprintType)
struct FStatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlayerStat PlayerStatEnum = EPlayerStat::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Level = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Cost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsMax = false;
};

USTRUCT(BlueprintType)
struct FStruct_StatDisplays 
{
	GENERATED_BODY()

	float Health; 
	float MaxHealth; 
	float Mana; 
	float MaxMana; 
	float Stamina; 
	float MaxStamina;
};

USTRUCT()
struct FStruct_Progression : public FTableRowBase
{
	GENERATED_BODY()

	FStruct_Progression() = default;
	FStruct_Progression(int level, float amount, int levelUpCost)
	{
		Level = level;
		Amount = amount;
		LevelupCost = levelUpCost;
	}

	UPROPERTY(EditAnywhere)
	int Level = 0;
	UPROPERTY(EditAnywhere)
	float Amount = 0.f;
	UPROPERTY(EditAnywhere)
	int LevelupCost = 0;
};

//Store current player level 
USTRUCT()
struct FStruct_Level : public FTableRowBase
{
	GENERATED_BODY()

	FStruct_Level() = default;

	FStruct_Level(FName name, int level)
	{
		StatName = name;
		Level = level;
	}

	UPROPERTY(EditAnywhere)
	FName StatName;

	UPROPERTY(EditAnywhere)
	int Level = 1;
};


//USTRUCT()
//struct FStruct_StatLevels : public FTableRowBase
//{
//	GENERATED_BODY()
//	
//	UPROPERTY(EditAnywhere)
//	float Health = 100.f; 
//	UPROPERTY(EditAnywhere)
//	float Mana = 90.f; 
//	UPROPERTY(EditAnywhere)
//	float Stamina = 80.f;
//};