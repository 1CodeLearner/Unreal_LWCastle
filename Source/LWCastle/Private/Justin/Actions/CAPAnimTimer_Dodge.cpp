// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAPAnimTimer_Dodge.h"
#include "Justin/AComponents/CGameplayComponent.h"

void UCAPAnimTimer_Dodge::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	auto AttTemp = GameplayComp->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
	if (AttTemp)
	{
		AttComp = AttTemp;
	}
}

void UCAPAnimTimer_Dodge::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (IsMontagePlaying())
		return;

	StartMontage(this);
	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UCAPAnimTimer_Dodge::OnMontageEnd);
	GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, Montage);

	FScriptDelegate Script;
	Script.BindUFunction(this, "OnStaminaDepleted");
	AttComp->OnStaminaDepleted.AddUnique(Script);

	AttComp->SpendStamina(StaminaSpendAmount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BlockedTags.ToString());
}

void UCAPAnimTimer_Dodge::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);

	if (AttComp->OnStaminaDepleted.IsBound())
	{
		AttComp->OnStaminaDepleted.Clear();
	}
}

void UCAPAnimTimer_Dodge::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	StopMontage(this);

	if (AttComp->OnStaminaDepleted.IsBound())
	{
		AttComp->OnStaminaDepleted.Clear();
	}
}

bool UCAPAnimTimer_Dodge::CanStart_Implementation(AActor* InstigatorActor, UCAction* StartingAction) const
{
	if (!BlockedTags.IsEmpty())
	{
		return false;
	}
	else if(AttComp->GetCurrentStamina() <= 0.f)
	{
		GetGameplayComponent()->AddAction(GetGameplayComponent()->GetOwner(), StunActionClass);
		return false;
	}

	return Super::CanStart_Implementation(InstigatorActor, StartingAction);

}

void UCAPAnimTimer_Dodge::OnMontageEnd(UAnimMontage* EndedMontage, bool bInterrupted)
{
	GetGameplayComponent()->CompleteActionBy(GetGameplayComponent()->GetOwner(), this);
}

void UCAPAnimTimer_Dodge::OnStaminaDepleted()
{
	if (ensure(StunActionClass))
	{
		GetGameplayComponent()->AddAction(GetGameplayComponent()->GetOwner(), StunActionClass);
	}

	if (AttComp->OnStaminaDepleted.IsBound())
	{
		AttComp->OnStaminaDepleted.Clear();
	}
}


void UCAPAnimTimer_Dodge::AddBlockedTag(FGameplayTagContainer AddedTags)
{
	if (!BlockedTags.HasAny(AddedTags))
		BlockedTags.AppendTags(AddedTags);
}

void UCAPAnimTimer_Dodge::RemoveBlockedTag(FGameplayTagContainer AddedTags)
{
	if (BlockedTags.HasAnyExact(AddedTags))
		BlockedTags.RemoveTags(AddedTags);
}
