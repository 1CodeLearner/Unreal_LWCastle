// Fill out your copyright notice in the Description page of Project Settings.


#include "Uwol/PlayerAnim.h"
#include "Uwol/uwol_test.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "GameplayTagContainer.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"

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

		checkattack = player->isattackingmagic;
		auto Gameplay = player->GetComponentByClass<UCGameplayComponent>();
		if (Gameplay)
		{
			bIsAiming = Gameplay->ActiveGameplayTags.HasTagExact(FGameplayTag::RequestGameplayTag("Combat.Magic")) || player->bSniperAim;
		}

		bIsDead = !player->GetComponentByClass<UCPlayerAttributeComp>()->IsAlive();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

