// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_BossHealth.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UUserWidget_BossHealth : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AController* OwningPlayer;
};
