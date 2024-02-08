// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAPAnimTimer_StunHard.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Uwol/uwol_test.h"
#include "Justin/AComponents/CAttributeComponent.h"

void UCAPAnimTimer_StunHard::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (IsMontagePlaying())
	{
		UnbindDelegate();
		StopMontage(this);
		ClearTimer();
	}

	auto Player = Cast<Auwol_test>(GetOuter());
	if (ensure(Player))
	{
		Player->bIsStunned = true;

		if (GetGameplayComponent()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Movement.Roll")))
		{
			Player->EndTeleport();
		}

		StartMontage(this);
		if (ensureAlways(!GetAnimInstance()->Montage_GetBlendingOutDelegate()->IsBound()))
		{
			MontageEndDelegate.BindUObject(this, &UCAPAnimTimer_StunHard::OnMontageEnd);
			GetAnimInstance()->Montage_SetBlendingOutDelegate(MontageEndDelegate, Montage);
		}

		StartTimer(this);
	}
}

void UCAPAnimTimer_StunHard::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);
	ClearTimer();
}

void UCAPAnimTimer_StunHard::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);
	StopMontage(this);
	ClearTimer();
}

bool UCAPAnimTimer_StunHard::CanStart_Implementation(AActor* InstigatorActor, UCAction* StartingAction) const
{
	auto Comp = InstigatorActor->GetComponentByClass<UCAttributeComponent>();
	if(Comp)
	{
		return Comp->IsAlive() && (StartingAction == this || Super::CanStart_Implementation(InstigatorActor, StartingAction));
	}
	return false;
}

void UCAPAnimTimer_StunHard::OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted)
{
	GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);

	UnbindDelegate();

	auto Player = Cast<Auwol_test>(GetOuter());
	if (Player)
	{
		Player->bIsStunned = false;
	}
}

void UCAPAnimTimer_StunHard::ExecuteAction(AActor* InstigatorActor)
{
	auto Player = Cast<Auwol_test>(InstigatorActor);
	if (Player)
	{
		Player->bIsStunned = false;
	}
}

void UCAPAnimTimer_StunHard::UnbindDelegate()
{
	auto Delegate = GetAnimInstance()->Montage_GetBlendingOutDelegate();

	if (Delegate && Delegate->IsBound())
	{
		GetAnimInstance()->Montage_GetBlendingOutDelegate()->Unbind();
	}
}
