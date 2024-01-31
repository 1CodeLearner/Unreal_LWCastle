// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CAPAnimTimer.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "GameFramework/Character.h"

float UCAPAnimTimer::GetAnimMontageLength()
{
	if (Montage && AnimInstance->Montage_IsPlaying(Montage))
	{
		int32 SecIndex = Montage->GetSectionIndex(MontageSection);
		return Montage->GetSectionLength(SecIndex);
	}
	return -1.f;
}

void UCAPAnimTimer::Initialize_Implementation(UCGameplayComponent* GameplayComp)
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

void UCAPAnimTimer::StartMontage(UCAPAnimTimer* AnimTimer)
{
	if (AnimInstance) {
		//Making sure Anim Montage has notify available
		if (Montage->IsNotifyAvailable()) {
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(AnimTimer, &UCAPAnimTimer::OnNotifyBegin);
			AnimInstance->Montage_Play(Montage, InPlayRate);
		}

		if (!MontageSection.IsNone())
			AnimInstance->Montage_JumpToSection(MontageSection, Montage);
	}
}

void UCAPAnimTimer::StopMontage(UCAPAnimTimer* AnimTimer)
{
	if (AnimInstance) {
		if (AnimInstance->OnPlayMontageNotifyBegin.Contains(AnimTimer, "OnNotifyBegin"))
		{
			AnimInstance->OnPlayMontageNotifyBegin.Remove(AnimTimer, "OnNotifyBegin");
		}
		AnimInstance->Montage_Stop(InBlendOutTime, Montage);
	}
}

bool UCAPAnimTimer::IsMontagePlaying() const
{
	return AnimInstance->Montage_IsActive(Montage);
}

void UCAPAnimTimer::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("Testing OnNotifyBegin override"));
}

UAnimInstance* UCAPAnimTimer::GetAnimInstance() const
{
	return AnimInstance;
}

void UCAPAnimTimer::PauseTimer()
{
	GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
}

void UCAPAnimTimer::UnPauseTimer()
{
	GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
}

void UCAPAnimTimer::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool UCAPAnimTimer::IsTimerValid()
{
	return TimerHandle.IsValid();
}

void UCAPAnimTimer::StartTimer(UCAPAnimTimer* AnimTimer)
{
	TimerDelegate.BindUFunction(AnimTimer, "ExecuteAction");
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimerDuration, false);
}

void UCAPAnimTimer::ExecuteAction(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Testing ExecuteAction override"));
}

UCAPAnimTimer::UCAPAnimTimer()
{
	TimerDuration = 0.f;
	InBlendOutTime = 0.f;
	InPlayRate = 0.f;
}
