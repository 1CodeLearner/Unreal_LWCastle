// Fill out your copyright notice in the Description page of Project Settings.


#include "Uwol/PlayerAnim.h"
#include "Uwol/uwol_test.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<Auwol_test>(ownerPawn);

	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		speed = FVector::DotProduct(forwardVector, velocity);

		// 좌우 속도
		FVector rightVector = player->GetActorRightVector();
		verdirection = FVector::DotProduct(rightVector, velocity);

		// 공중
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();

		checkdodge = player->IsDodging;
		checkattack = player->isattackingmagic;

	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

