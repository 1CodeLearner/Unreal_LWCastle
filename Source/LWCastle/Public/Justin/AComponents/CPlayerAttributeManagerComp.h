// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Justin/PlayerStatTypes.h"
#include "CPlayerAttributeManagerComp.generated.h"

USTRUCT()
struct FStatProgressConversion
{
	GENERATED_BODY()

	FStatProgressConversion() = default;
	FStatProgressConversion(TArray<FStruct_Progression> _Holder) {
		ProgressionHolder = _Holder;
	}

	UPROPERTY()
	TArray<FStruct_Progression> ProgressionHolder;
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

	void BeginInit();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite)
	FPlayerStatUpdatedDelegate OnPlayerStatUpdated;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FMaxReachedDelegate OnMaxReached;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerStat(EPlayerStat PlayerStatType);	

	UFUNCTION(BlueprintCallable)
	FStatInfo GetStatInfoOf(EPlayerStat StatType) const;
	UFUNCTION()
	FStruct_StatDisplays GetAllStats() const;

	UFUNCTION(BlueprintCallable)
	int GetHealthLevel() const;
	UFUNCTION(BlueprintCallable)
	int GetManaLevel() const;
	UFUNCTION(BlueprintCallable)
	int GetStaminaLevel() const;

protected:
	virtual void BeginPlay() override;
private:

	UPROPERTY()
	TObjectPtr<UCInventoryComponent> InventoryComp;
	//Later add Elemental Magic Manager Component

	//Container for Storing player's current level for each attributes
	UPROPERTY(VisibleAnywhere, Category = "PlayerAttribute")
	TMap<FName, FStruct_Level> PlayerLevelMap;
	//StatName, Stat Progression
	UPROPERTY(VisibleAnywhere, Category = "PlayerAttribute")
	TMap<FName, FStatProgressConversion > PlayerProgressionMap;

private:
	void IncrementStatLevel(FName StatName);
	bool IsMaxReached(FName StatName);

	FStruct_Progression GetCurrentProgressionOf(FName StatName);
	FStruct_Progression GetCurrentProgressionOf(FName StatName) const;
	FStruct_Progression GetLastProgressionOf(FName StatName);
	int GetCurrentProgressionIndex(FName StatName);
	int GetCurrentProgressionIndex(FName StatName) const;
	int GetLevelupCostFor(FName StatName);
	bool CheckIsMaxFor(FName StatName);
	bool CheckIsMaxFor(FName StatName) const;
	FName GetStatName(EPlayerStat PlayerStatEnum);
	FName GetStatName(EPlayerStat PlayerStatEnum) const;
};
