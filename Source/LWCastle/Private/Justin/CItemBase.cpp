// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CItemBase.h"
#include "Justin/CGameModeBase.h"

void UCItemBase::Start_Implementation(AActor* InstigatorActor)
{	

}

void UCItemBase::Stop_Implementation(AActor* InstigatorActor)
{
}

void UCItemBase::Pause_Implementation(AActor* InstigatorActor)
{

}

UWorld* UCItemBase::GetWorld() const
{
	ACGameModeBase* GM = Cast<ACGameModeBase>(GetOuter());
	if (GM) 
	{
		return GM->GetWorld();
	}
	return nullptr;
}
