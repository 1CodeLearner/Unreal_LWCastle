// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPlayerAttributeManagerComp.generated.h"

USTRUCT()
struct FPlayerLevel : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Level = 1;
};

USTRUCT()
struct FStruct_PlayerAttribute: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Level = 0;
	UPROPERTY(EditAnywhere)
	float Amount = 0.f;
	UPROPERTY(EditAnywhere)
	int LevelupCost = 0;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCPlayerAttributeManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPlayerAttributeManagerComp();

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
	TObjectPtr<UDataTable> DT_PlayerLevels;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerHealthList;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerManaList;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAttribute")
	TObjectPtr<UDataTable> DT_PlayerStaminaList;

};
