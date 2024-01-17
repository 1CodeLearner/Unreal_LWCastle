// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/Action_TestUpDown.h"

void UAction_TestUpDown::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	UE_LOG(LogTemp, Warning, TEXT("Inside Start"));

	ExecuteMagicDelegate.BindUFunction(this, "ExecuteMagic", InstigatorActor);
	GetWorld()->GetTimerManager().SetTimer(ExecuteMagicHandle, ExecuteMagicDelegate, .2, true);
}

void UAction_TestUpDown::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	UE_LOG(LogTemp, Warning, TEXT("Inside Stop"));

	GetWorld()->GetTimerManager().ClearTimer(ExecuteMagicHandle);
	Super::StopAction_Implementation(InstigatorActor);
}

void UAction_TestUpDown::ResetMagic(AActor* InstigatorActor)
{
	StopAction(InstigatorActor);

}

void UAction_TestUpDown::ExecuteMagic(AActor* InstigatorActor)
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

