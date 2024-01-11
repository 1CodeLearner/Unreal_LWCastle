// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Justin/PlayerStatTypes.h"
#include "CGameModeBase.generated.h"

class UCItemBase;
/**
 *
 */
UCLASS()
class LWCASTLE_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	UClass* GetItemClassByName(FName ItemName);
	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<UCItemBase>> GetItems();

	UFUNCTION()
	UDataTable* GetCurrentLevelsTable();
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	UDataTable* DT_CurrentLevels;

	UFUNCTION()
	UDataTable* GetProgressionTableOf(EPlayerStat StatType);
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	TObjectPtr<UDataTable> DT_HealthProgression;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	TObjectPtr<UDataTable> DT_ManaProgression;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	TObjectPtr<UDataTable> DT_StaminaProgression;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TArray<TSubclassOf<UCItemBase>> ItemBaseClasses;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TMap<FName, TSubclassOf<UCItemBase>> ItemBaseMap;


};
