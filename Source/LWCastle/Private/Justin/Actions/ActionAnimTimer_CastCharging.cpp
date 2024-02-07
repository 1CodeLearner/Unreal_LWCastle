// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/ActionAnimTimer_CastCharging.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AudioDevice.h"
#include "ActiveSound.h"

UActionAnimTimer_CastCharging::UActionAnimTimer_CastCharging()
{
	ManaChargingRate = 1.0f;
	AudioComp = nullptr;
	bIsSoundPaused = false;
	DurationElapsed = 0.f;
}

void UActionAnimTimer_CastCharging::Tick(float DeltaTime)
{
	if (PlayerAttribute && PlayerAttribute->TryChannelMana(ManaChargingRate * DeltaTime))
	{
		if (bIsSoundPaused)
		{
			bIsSoundPaused = false;
			AudioComp->SetPaused(false);
		}
		CombatComp->AddChannelMana(ManaChargingRate * DeltaTime);
	}
	else
	{
		if (!bIsSoundPaused)
		{
			bIsSoundPaused = true;
			AudioComp->SetPaused(true);
		}
	}
}

TStatId UActionAnimTimer_CastCharging::GetStatId() const
{
	TStatId Stat;
	return Stat;
}

bool UActionAnimTimer_CastCharging::IsTickable() const
{
	return true;
}

bool UActionAnimTimer_CastCharging::IsAllowedToTick() const
{
	return StartTick;
}

void UActionAnimTimer_CastCharging::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);

	auto Attribute = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCPlayerAttributeComp>();
	if (Attribute)
	{
		PlayerAttribute = Attribute;
	}
	auto CombatCompTemp = GetGameplayComponent()->GetOwner()->GetComponentByClass<UCCombatComponent>();
	if (CombatCompTemp)
	{
		CombatComp = CombatCompTemp;
	}
}

void UActionAnimTimer_CastCharging::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	if (ensure(!IsTimerValid() && !IsMontagePlaying()))
	{
		StartMontage(this);
	}
	CombatComp->StartChannelMana();

	if (!CombatComp->OnManaCharged.IsBound())
		CombatComp->OnManaCharged.BindUFunction(this, "ExecuteAction");

	UGameplayStatics::PlaySound2D(this, ChargingStartSound);
	auto Actor = Cast<AActor>(GetOuter());
	if (ensure(Actor))
	{
		if (AudioComp)
		{
			AudioComp = nullptr;
		}

		AudioComp = UGameplayStatics::SpawnSoundAttached(ChargingSound, Actor->GetRootComponent());

		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "OnSoundPercent");
		AudioComp->OnAudioPlaybackPercent.AddUnique(Delegate);

		AudioComp->SetPaused(true);
	}

}

void UActionAnimTimer_CastCharging::CompleteAction_Implementation(AActor* InstigatorActor)
{
	Super::CompleteAction_Implementation(InstigatorActor);

	if (IsMontagePlaying())
		StopMontage(this);

	if (AudioComp)
	{
		AudioComp->Stop();
		AudioComp->OnAudioPlaybackPercent.RemoveAll(this);
	}

	DurationElapsed = 0.f;
}

void UActionAnimTimer_CastCharging::PauseAction_Implementation(AActor* InstigatorActor)
{
	Super::PauseAction_Implementation(InstigatorActor);
	UnbindNotifyEvent(this);
	StartTick = false;
	AudioComp->SetPaused(true);
}

void UActionAnimTimer_CastCharging::UnPauseAction_Implementation(AActor* InstigatorActor)
{
	Super::UnPauseAction_Implementation(InstigatorActor);
	StartMontage(this);
}


void UActionAnimTimer_CastCharging::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);
	StopMontage(this);

	CombatComp->ResetChannelMana();
	StartTick = false;
	PlayerAttribute->CancelChannelingMana();
	if (CombatComp->OnManaCharged.IsBound())
		CombatComp->OnManaCharged.Clear();

	AudioComp->Stop();
	AudioComp->OnAudioPlaybackPercent.RemoveAll(this);
	DurationElapsed = 0.f;
}

void UActionAnimTimer_CastCharging::ExecuteAction(AActor* InstigatorActor)
{
	Super::ExecuteAction(InstigatorActor);

	UE_LOG(LogTemp, Warning, TEXT("EXECUTING EACLKADJ ACTION!"));

	StartTick = false;
	CombatComp->CompleteChannelMana();
	PlayerAttribute->CompleteChannelingMana();
	if (CombatComp->OnManaCharged.IsBound())
		CombatComp->OnManaCharged.Clear();
	GetGameplayComponent()->CompleteActionBy(InstigatorActor, this);
	GetGameplayComponent()->StartActionByName(InstigatorActor, "ChargedState");
}

void UActionAnimTimer_CastCharging::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::OnNotifyBegin(NotifyName, BranchingPointPayload);

	UnbindNotifyEvent(this);
	StartTick = true;
	AudioComp->SetPaused(false);
}

void UActionAnimTimer_CastCharging::OnSoundPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	if (PlaybackPercent == 0.f)
		return;

	float StartTime = AudioComp->Sound->GetMaxDistance() * PlaybackPercent;
	AudioComp->FadeIn(.5f, 1.f, StartTime);
}