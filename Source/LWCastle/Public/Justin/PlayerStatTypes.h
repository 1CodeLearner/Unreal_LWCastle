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

//Store current player level 
USTRUCT()
struct FStruct_PlayerLevel : public FTableRowBase
{
	GENERATED_BODY()

	FStruct_PlayerLevel() = default;

	FStruct_PlayerLevel(FName name, int level)
	{
		StatName = name;
		Level = level;
	}

	UPROPERTY(EditAnywhere)
	FName StatName;

	UPROPERTY(EditAnywhere)
	int Level = 1;
};