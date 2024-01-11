// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Justin/PlayerStatTypes.h"
#include "CPlayerAttributeManagerComp.generated.h"

//Store player progression data
USTRUCT()
struct FStruct_PlayerAttribute : public FTableRowBase
{
	GENERATED_BODY()

	FStruct_PlayerAttribute() = default;
	FStruct_PlayerAttribute(int level, float amount, int levelUpCost)
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

USTRUCT()
struct FStatProgressConversion
{
	GENERATED_BODY()

	FStatProgressConversion() = default;
	FStatProgressConversion(TArray<FStruct_PlayerAttribute> _Holder) {
		ProgressionHolder = _Holder;
	}

	UPROPERTY()
	TArray<FStruct_PlayerAttribute> ProgressionHolder;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStatUpdatedDelegate, FStatInfo, StatInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxReachedDelegate, EPlayerStat, PlayerStatEnum);

class UCInventoryComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCPlayerAttributeManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPlayerAttributeManagerComp();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite)
	FPlayerStatUpdatedDelegate OnPlayerStatUpdated;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FMaxReachedDelegate OnMaxReached;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerStat(EPlayerStat PlayerStatType);

	UFUNCTION(BlueprintCallable)
	FStatInfo GetStatInfo(EPlayerStat StatType) const;

	UFUNCTION(BlueprintCallable)
	int GetHealthLevel() const;
	UFUNCTION(BlueprintCallable)
	int GetManaLevel() const;
	UFUNCTION(BlueprintCallable)
	int GetStaminaLevel() const;

private:

	UPROPERTY()
	TObjectPtr<UCInventoryComponent> InventoryComp;
	//Later add Elemental Magic Manager Component

	//Container for Storing player's current level for each attributes
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TMap<FName, FStruct_PlayerLevel> PlayerLevelMap;
	//StatName, Stat Progression
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TMap<FName, FStatProgressConversion > PlayerProgressionMap;

private:
	FName GetStatName(EPlayerStat PlayerStatEnum);
	FName GetStatName(EPlayerStat PlayerStatEnum) const;
	void IncrementStatLevel(FName StatName);
	bool IsMaxReached(FName StatName);

	FStruct_PlayerAttribute GetCurrentProgressionOf(FName StatName);
	FStruct_PlayerAttribute GetCurrentProgressionOf(FName StatName) const;
	FStruct_PlayerAttribute GetLastProgressionOf(FName StatName);
	int GetCurrentProgressionIndex(FName StatName);
	int GetCurrentProgressionIndex(FName StatName) const;
	int GetLevelupCostFor(FName StatName);
	bool CheckIsMaxFor(FName StatName);
	bool CheckIsMaxFor(FName StatName) const;
};
