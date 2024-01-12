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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Mana; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMana; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Stamina; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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
