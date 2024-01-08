// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPlayerAttributeManagerComp.generated.h"

UENUM(BlueprintType)
enum class EPlayerStat : uint8
{
	NONE,
	HEALTH,
	MANA,
	STAMINA
};

UENUM(BlueprintType)
enum class EFailReason : uint8
{
	NONE,
	NOCURRENCY,
	MAXREACHED
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerStatUpdatedDelegate, EPlayerStat, PlayerStatEnum, int, Level, int, Cost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxReachedDelegate, EPlayerStat, PlayerStatEnum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatUpdateFailedDelegate, EFailReason, FailReason);


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
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FStatUpdateFailedDelegate OnStatUpdateFailed;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TryUpdatePlayerStat(EPlayerStat PlayerStatType);

	UFUNCTION(BlueprintCallable)
	int GetLevel(FName StatName) const;

	UFUNCTION(BlueprintCallable)
	int GetHealthLevel() const;
	UFUNCTION(BlueprintCallable)
	int GetManaLevel() const;
	UFUNCTION(BlueprintCallable)
	int GetStaminaLevel() const;

	UFUNCTION(BlueprintCallable)
	int GetHealthLevelupCost() const;
	UFUNCTION(BlueprintCallable)
	int GetManaLevelupCost() const;
	UFUNCTION(BlueprintCallable)
	int GetStaminaLevelupCost() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerHealthList;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerManaList;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerStaminaList;

private:
	//<StatName, Current Stat Level>
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TMap<FName, FStruct_PlayerLevel> PlayerLevelMap;
	//<StatName, Stat Progression> 
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TMap<FName, FStatProgressConversion > PlayerProgressionMap;

	FName GetStatName(EPlayerStat PlayerStatEnum);
	void IncrementStatLevel(FName StatName);
	bool IsMaxReached(FName StatName);

	FStruct_PlayerAttribute GetCurrentProgressionOf(FName StatName);
	FStruct_PlayerAttribute GetLastProgressionOf(FName StatName);
	int GetCurrentProgressionIndex(FName StatName);
};
