// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/Action_TestUpDown.h"
#include "Justin/AComponents/CGameplayComponent.h"

void UAction_TestUpDown::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	UE_LOG(LogTemp, Warning, TEXT("Inside Start"));

	ExecuteMagicDelegate.BindUFunction(this, "ExecuteMagic", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(ExecuteMagicHandle, ExecuteMagicDelegate, .05, true);
}

void UAction_TestUpDown::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	UE_LOG(LogTemp, Warning, TEXT("Inside Stop"));

	GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
	Super::CompleteAction_Implementation(InstigatorActor);
}

/*
void UAction_TestUpDown::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	if(!PlayerAttComp)
	{
		auto CompTemp=  GameplayComp->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
		if(CompTemp)
		{
			PlayerAttComp = CompTemp;
		}
	}
}*/

void UAction_TestUpDown::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	
	GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
}

void UAction_TestUpDown::ResetMagic(AActor* InstigatorActor)
{
	CompleteAction(InstigatorActor);

}

void UAction_TestUpDown::ExecuteMagic_Implementation(AActor* InstigatorActor)
{
	FVector Origin = InstigatorActor->GetActorLocation();
	FVector Start = Origin + 100.f * InstigatorActor->GetActorForwardVector();
	FVector End = Start + 2000.f * InstigatorActor->GetActorForwardVector();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorActor);
	FHitResult Hit;
	FColor DebugColorLocal;
	bool Success = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.f, 2.f);

}

