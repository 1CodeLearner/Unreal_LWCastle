// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CPlayerController.h"
#include "Justin/Widgets/CChargeWidget.h"

ACPlayerController::ACPlayerController()
{
	StatManagerComp = CreateDefaultSubobject<UCPlayerAttributeManagerComp>(TEXT("StatManagerComp"));
}

void ACPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StatManagerComp->BeginInit();

}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ChargeWidgetClass) {
		auto Widget = CreateWidget<UCChargeWidget>(this, ChargeWidgetClass);
		if (Widget) {
			Widget->AddToViewport();
			ChargeWidget = Widget;
		}
	}
}
