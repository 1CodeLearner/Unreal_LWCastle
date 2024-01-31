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

UCLASS()
class LWCASTLE_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACPlayerController();

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
