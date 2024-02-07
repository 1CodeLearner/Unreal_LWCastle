// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AComponents/CPlayerAttributeManagerComp.h"
#include "CPlayerController.generated.h"

class UCChargeWidget;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFadeSuccessDelegate, AActor*, Owner);

UCLASS()
class LWCASTLE_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACPlayerController();

	UPROPERTY(BlueprintCallable, Category = "UI")
	FFadeSuccessDelegate OnFadeSuccess;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void StartFade(bool bIsFadeOut);

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "ChargeWidget")
	UCChargeWidget* ChargeWidget;

	void Initialize();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager")
	TObjectPtr<UCPlayerAttributeManagerComp> StatManagerComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	int value;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCChargeWidget> ChargeWidgetClass;
};
