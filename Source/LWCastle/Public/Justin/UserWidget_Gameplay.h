// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_Gameplay.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonClickedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonReturnDelegate);
UCLASS()
class LWCASTLE_API UUserWidget_Gameplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FButtonClickedDelegate OnButtonClicked;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FButtonReturnDelegate OnButtonReturned;
};
