// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CChargeWidget.generated.h"

class UProgressBar;
/**
 *
 */
UCLASS()
class LWCASTLE_API UCChargeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateWidget(float MaxMana, float CurrentMana);
	void SetVisibilityWidget(ESlateVisibility visibility);
	void ResetWidget();
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ChargeProgressBar;
};
