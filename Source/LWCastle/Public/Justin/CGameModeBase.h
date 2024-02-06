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
	TArray<FStruct_Progression> GetProgressions(EPlayerStat StatType) const;
	UFUNCTION()
	FStruct_Progression GetCurrentProgressionOf(EPlayerStat StatType, int Level);
	UFUNCTION()
	TArray<FStruct_Level> GetCurrentLevels() const;

	
	UFUNCTION()
	void SlowDownTime(AActor* ActorContext);
	UFUNCTION()
	void RestoreTime(AActor* ActorContext);

	UFUNCTION(BlueprintCallable, Category = "SpawnActor")
	void RespawnPlayer(AController* Controller);

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TArray<TSubclassOf<UCItemBase>> ItemBaseClasses;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TMap<FName, TSubclassOf<UCItemBase>> ItemBaseMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	UDataTable* DT_CurrentLevels;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	TObjectPtr<UDataTable> DT_HealthProgression;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	TObjectPtr<UDataTable> DT_ManaProgression;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerProgression")
	TObjectPtr<UDataTable> DT_StaminaProgression;

private:
	FName GetStatName(EPlayerStat PlayerStatEnum);
	FName GetStatName(EPlayerStat PlayerStatEnum) const;
};
