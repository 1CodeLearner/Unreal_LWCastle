// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionPause_Sprint.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"


UCActionPause_Sprint::UCActionPause_Sprint()
{
	StartTick = false;
	StaminaSpendRate = 1.0f;
	MaxSprintSpeed = 0.f;
	PrevSpeed = 0.0;
	Character = nullptr;
	isSprinting = false;
}

void UCActionPause_Sprint::Tick(float DeltaTime)
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

TStatId UCActionPause_Sprint::GetStatId() const
{
	TStatId Stat;
	return Stat;
}

bool UCActionPause_Sprint::IsTickable() const
{
	return true;
}

bool UCActionPause_Sprint::IsAllowedToTick() const
{
	return StartTick;
}

void UCActionPause_Sprint::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);
	auto Attribute = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
	if (Attribute)
	{
		AttributeComp = Attribute;
	}
}

bool UCActionPause_Sprint::CanStart_Implementation(AActor* InstigatorActor) const
{
	bool bRunning = Super::CanStart_Implementation(InstigatorActor);
	return bRunning || isSprinting;
}

void UCActionPause_Sprint::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (isSprinting)
	{
		GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);
	}
	else
	{
		if (!Character)
		{
			Character = Cast<ACharacter>(InstigatorActor);
		}

		if (Character && !Character->GetCharacterMovement()->IsFalling())
		{
			StartRunning();
		}
		else
		{
			if (!Character->LandedDelegate.Contains(this, "OnLand"))
			{
				Character->LandedDelegate.AddDynamic(this, &UCActionPause_Sprint::OnLand);
				isSprinting = true;
			}
		}
	}
}

void UCActionPause_Sprint::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	StopRunning();
}

void UCActionPause_Sprint::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
	PauseRunning();
}

void UCActionPause_Sprint::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	UnPauseRunning();
}

void UCActionPause_Sprint::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopRunning();
}

void UCActionPause_Sprint::StartRunning()
{
	if (!Character->LandedDelegate.Contains(this, "OnLand"))
	{
		Character->LandedDelegate.AddDynamic(this, &UCActionPause_Sprint::OnLand);
	}

	if (!AttributeComp->OnStaminaDepleted.Contains(this, "OnStaminaDepleted"))
	{
		AttributeComp->OnStaminaDepleted.AddDynamic(this, &UCActionPause_Sprint::OnStaminaDepleted);
	}

	PrevSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;

	StartTick = true;
	isSprinting = true;
}

void UCActionPause_Sprint::PauseRunning()
{
	StartTick = false;
}

void UCActionPause_Sprint::UnPauseRunning()
{
	StartTick = true;
}

void UCActionPause_Sprint::StopRunning()
{
	if (Character->LandedDelegate.Contains(this, "OnLand"))
	{
		Character->LandedDelegate.Remove(this, "OnLand");
	}

	if (AttributeComp->OnStaminaDepleted.Contains(this, "OnStaminaDepleted"))
	{
		AttributeComp->OnStaminaDepleted.Remove(this, "OnStaminaDepleted");
	}

	Character->GetCharacterMovement()->MaxWalkSpeed = PrevSpeed;

	PrevSpeed = 0.f;
	StartTick = false;
	isSprinting = false;
}

void UCActionPause_Sprint::OnStaminaDepleted()
{
	GetGameplayComponent()->AddAction(GetGameplayComponent()->GetOwner(), ActionEffectStunClass);
}

void UCActionPause_Sprint::OnLand(const FHitResult& Hit)
{
	if (!isSprinting)
	{
		StartRunning();
	}
	else
		StartTick = true;
}