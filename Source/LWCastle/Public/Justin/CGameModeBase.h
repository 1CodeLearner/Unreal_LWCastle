// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UCItemBase;
/**
 * 
 */
UCLASS()
class LWCASTLE_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TArray<TSubclassOf<UCItemBase>> ItemBaseClasses;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TMap<FName, TSubclassOf<UCItemBase>> ItemBaseMap;
};
	