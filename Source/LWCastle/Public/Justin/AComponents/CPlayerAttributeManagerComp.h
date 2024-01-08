// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPlayerAttributeManagerComp.generated.h"

USTRUCT(BlueprintType)
struct FStruct_PlayerLevel : public FTableRowBase
{
	GENERATED_BODY()

	FStruct_PlayerLevel() = default;

	FStruct_PlayerLevel(FName name, int level)
	{
		StatName = name;
		Level = level;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 1;
};

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
	FStatProgressConversion(TMap<FName, FStruct_PlayerAttribute> _Holder) {
		ProgressionHolder = _Holder;
	}

	UPROPERTY()
	TMap<FName, FStruct_PlayerAttribute> ProgressionHolder;
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerStatUpdatedDelegate);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCPlayerAttributeManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPlayerAttributeManagerComp();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite)
	FPlayerStatUpdatedDelegate OnPlayerStatUpdated;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerStat(FStruct_PlayerLevel UpdatedLevel);

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

	//bool CanLevelUp(FString StatName);

	//FStruct_PlayerAttribute GetCurrentAttributeOf(FString StatName);
	//FStruct_PlayerAttribute GetLastAttributeOf(FString StatName);
	//FName GetCurrentProgressionKey(FString StatName);


	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TMap<FName, FStruct_PlayerLevel> PlayerLevelMap;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TMap<FName, FStatProgressConversion > PlayerProgressionMap;


	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerHealthList;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerManaList;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerStaminaList;

};
