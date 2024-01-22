// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/Action_Dodge.h"
#include "GameFramework/Character.h"

void UAction_Dodge::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);

	if (Character)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(AnimationMontage, MontageRate);
	}
}