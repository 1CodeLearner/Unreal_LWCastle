// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionAnimTimer.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "GameFramework/Character.h"

float UCActionAnimTimer::GetAnimMontageLength()
{
	if (Montage && AnimInstance->Montage_IsPlaying(Montage))
	{
		int32 SecIndex = Montage->GetSectionIndex(MontageSection);
		return Montage->GetSectionLength(SecIndex);
	}
	return -1.f;
}

void UCActionAnimTimer::Initialize_Implementation(UCGameplayComponent* GameplayComp)
{
	Super::Initialize_Implementation(GameplayComp);
	UAnimInstance* Anim;
	ACharacter* CharacterTemp = Cast<ACharacter>(GameplayComp->GetOwner());
	if (CharacterTemp)
	{
		Anim = CharacterTemp->GetMesh()->GetAnimInstance();

		if (ensure(Anim))
		{
			AnimInstance = Anim;
		}
	}
}

void UCActionAnimTimer::StartMontage(UCActionAnimTimer* AnimTimer)
{
	if (AnimInstance && ensureAlways(Montage)) {
		//Making sure Anim Montage has notify available
		AnimInstance->Montage_Play(Montage, InPlayRate);
		if (Montage->IsNotifyAvailable() &&
			!AnimInstance->OnPlayMontageNotifyBegin.Contains(AnimTimer, "OnNotifyBegin"))
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(AnimTimer, &UCActionAnimTimer::OnNotifyBegin);
		}

		if (!MontageSection.IsNone())
			AnimInstance->Montage_JumpToSection(MontageSection, Montage);
	}
}

void UCActionAnimTimer::StopMontage(UCActionAnimTimer* AnimTimer)
{
	if (AnimInstance) {
		UnbindNotifyEvent(AnimTimer);
		AnimInstance->Montage_Stop(InBlendOutTime, Montage);
	}
}

void UCActionAnimTimer::UnbindNotifyEvent(UCActionAnimTimer* AnimTimer)
{
	if (AnimInstance)
	{
		if (AnimInstance->OnPlayMontageNotifyBegin.IsBound())
		{
			AnimInstance->OnPlayMontageNotifyBegin.Remove(AnimTimer, "OnNotifyBegin");
		}
	}
}

bool UCActionAnimTimer::IsMontagePlaying() const
{
	if (ensure(AnimInstance && Montage))
	{
		return AnimInstance->Montage_IsActive(Montage);
	}
	return false;
}

void UCActionAnimTimer::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("Testing OnNotifyBegin override"));
}

UAnimInstance* UCActionAnimTimer::GetAnimInstance() const
{
	return AnimInstance;
}

void UCActionAnimTimer::PauseTimer()
{
	GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
}

void UCActionAnimTimer::UnPauseTimer()
{
	GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
}

void UCActionAnimTimer::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool UCActionAnimTimer::IsTimerValid()
{
	return TimerHandle.IsValid();
}

float UCActionAnimTimer::GetTimerDuration()
{
	return TimerDuration;
}

float UCActionAnimTimer::GetTimerRemaining()
{
	return GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle);
}

void UCActionAnimTimer::StartTimer(UCActionAnimTimer* AnimTimer)
{
	TimerDelegate.BindUFunction(AnimTimer, "ExecuteAction", GetOuter());
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimerDuration, false);
}

void UCActionAnimTimer::ExecuteAction(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Testing ExecuteAction override"));
}


UCActionAnimTimer::UCActionAnimTimer()
{
	TimerDuration = 0.f;
	InBlendOutTime = 0.f;
	InPlayRate = 1.f;
}
