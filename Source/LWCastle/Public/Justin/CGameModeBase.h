// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Justin/PlayerStatTypes.h"
#include "LocationVolume.h"
#include "CGameModeBase.generated.h"

class UCItemBase;
class ATriggerBox;
class ULevelSequence;
class Auwol_test;
class AEnemyCharacter;
class UUserWidget_Gameplay;
class UUserWidget_BossHealth;

/**
 *
 */
UCLASS()
class LWCASTLE_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

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

	//UFUNCTION(BlueprintCallable, Category = "SpawnActor")
	//void RespawnPlayer(AController* Controller);

protected:

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

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DodgeSound;

	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	ULevelSequence* BossIntroSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TSubclassOf<UUserWidget_Gameplay> Widget_GameOverClass;
	UPROPERTY()
	UUserWidget_Gameplay* Widget_GameOver;

	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TSubclassOf<UUserWidget_Gameplay> Widget_VictoryClass;
	UPROPERTY()
	UUserWidget_Gameplay* Widget_Victory;

	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TSubclassOf<UUserWidget_BossHealth> Widget_BossHealthClass;
	UPROPERTY()
	UUserWidget_BossHealth* Widget_BossHealth;


private:
	FName GetStatName(EPlayerStat PlayerStatEnum);
	FName GetStatName(EPlayerStat PlayerStatEnum) const;

	bool bIsFirstTimeLoading = true;

	UPROPERTY()
	TArray<ATriggerBox*> TriggerBoxes;

	UPROPERTY()
	AEnemyCharacter* BossCharacter;
	UPROPERTY()
	Auwol_test* PlayerCharacter;
	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TSubclassOf<AActor> BlockingWallClass;
	UPROPERTY()
	AActor* BlockingWall;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector PlayerLocation_Battle;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector BossLocation_Battle;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector BlockingWallLocation;

	void IntroFade();

	UFUNCTION()
	void OnTriggerBoxOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void PlayerPauseOverlapsed();

	UFUNCTION()
	void OnSequenceFinished();

	UFUNCTION()
	void OnPlayerDead();

	UFUNCTION()
	void OnBossDead();

	UFUNCTION()
	void OnFadeSuccess_BattleStart(AActor* Actor);

	UFUNCTION()
	void PausePeriod();
	UFUNCTION()
	void PausePeriodWon();

	UFUNCTION()
	void ButtonClick_BattleReset();
	UFUNCTION()
	void OnFadeSuccess_BattleReset(AActor* Actor);

	UFUNCTION()
	void ButtonClick_BattleOver();
	UFUNCTION()
	void OnFadeSuccess_BattleOver(AActor* Actor);

	void PlayCinematic();
	void SetupBindings();
	void StartBattle();
	UFUNCTION()
	void SetBossStartState();
	void ResetBattle();
};
