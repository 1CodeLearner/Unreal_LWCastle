// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CPlayerController.h"

ACPlayerController::ACPlayerController()
{
	StatManagerComp = CreateDefaultSubobject<UCPlayerAttributeManagerComp>(TEXT("StatManagerComp"));
}

void ACPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StatManagerComp->BeginInit();
}
