// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Widgets/CChargeWidget.h"
#include "Components/Progressbar.h"

void UCChargeWidget::UpdateWidget(float MaxTime, float CurrentTime)
{
	if (ensure(ChargeProgressBar))
	{
		ChargeProgressBar->SetPercent(CurrentTime / MaxTime);
	}
}

void UCChargeWidget::SetVisibilityWidget(ESlateVisibility visibility)
{
	ChargeProgressBar->SetVisibility(visibility);
}

void UCChargeWidget::ResetWidget()
{
	ChargeProgressBar->SetPercent(0.f);

	ChargeProgressBar->SetVisibility(ESlateVisibility::Collapsed);
}

void UCChargeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ChargeProgressBar->SetVisibility(ESlateVisibility::Collapsed);
}
