// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Widgets/CChargeWidget.h"
#include "Components/Progressbar.h"

void UCChargeWidget::Update(float MaxTime, float CurrentTime)
{
	if (ensure(ChargeProgressBar)) 
	{
		ChargeProgressBar->SetPercent(CurrentTime / MaxTime);
	}
}

void UCChargeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ChargeProgressBar->SetVisibility(ESlateVisibility::Collapsed);
}
