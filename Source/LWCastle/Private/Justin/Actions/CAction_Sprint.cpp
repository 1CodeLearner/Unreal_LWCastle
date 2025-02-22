// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAction_Sprint.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "GameFramework/Character.h"

UCAction_Sprint::UCAction_Sprint()
{
	StartTick = false;
	StaminaSpendRate = 1.0f;
	MaxSprintSpeed = 0.f;
}

void UCAction_Sprint::Tick(float DeltaTime)
{
	if (Character->GetCharacterMovement()->Velocity.SquaredLength() <= 0.f) 
	{
		StartTick = false;
		GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);
	}
	else if (AttributeComp)
	{
		AttributeComp->SpendStamina(StaminaSpendRate * DeltaTime);
	}
}

TStatId UCAction_Sprint::GetStatId() const
{
	TStatId Stat;
	return Stat;
}

bool UCAction_Sprint::IsTickable() const
{
	return true;
}

bool UCAction_Sprint::IsAllowedToTick() const
{
	return StartTick;
}


void UCAction_Sprint::OnStaminaDepleted()
{
	StartTick = false;
	GetGameplayComponent()->AddAction(GetGameplayComponent()->GetOwner(), ActionEffectStunClass);
}

void UCAction_Sprint::OnLand(const FHitResult& Hit)
{
	PrevSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
}

void UCAction_Sprint::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);
	auto Attribute = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
	if (Attribute)
	{
		AttributeComp = Attribute;
	}
}

void UCAction_Sprint::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	AttributeComp->OnStaminaDepleted.AddDynamic(this, &UCAction_Sprint::OnStaminaDepleted);
	StartTick = true;
	Character = Cast<ACharacter>(InstigatorActor);
	Character->LandedDelegate.AddDynamic(this, &UCAction_Sprint::OnLand);
	if (Character)
	{
		PrevSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
	}
}

void UCAction_Sprint::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	AttributeComp->OnStaminaDepleted.Remove(this, "OnStaminaDepleted");
	StartTick = false;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = PrevSpeed;
	}
}

void UCAction_Sprint::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	AttributeComp->OnStaminaDepleted.Remove(this, "OnStaminaDepleted");
	Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = PrevSpeed;
	}
	StartTick = false;
}
