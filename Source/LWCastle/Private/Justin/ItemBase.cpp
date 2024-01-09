// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/ItemBase.h"
#include "Justin/CGameMode.h"

UItemBase::UItemBase()
{

}

void UItemBase::Activate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemBase Activate"));
}

void UItemBase::DeActivate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemBase Deactivate"));

}

void UItemBase::Execute_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemBase Execute"));

}

UWorld* UItemBase::GetWorld() const
{
	ACGameMode* GameMode = Cast<ACGameMode>(GetOuter());
	if (GameMode) 
	{
		return GameMode->GetWorld();
	}
	return nullptr;
}
