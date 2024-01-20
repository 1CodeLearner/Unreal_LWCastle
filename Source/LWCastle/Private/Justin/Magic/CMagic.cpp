// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagic.h"
#include "Justin/CGameplayLibrary.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

void UCMagic::Press_Implementation(AActor* InstigatorActor)
{
	bIsPressing = true;
	UE_LOG(LogTemp, Warning, TEXT("Running Press %s"), *GetNameSafe(this));
}

void UCMagic::Release_Implementation(AActor* InstigatorActor)
{
	bIsPressing = false;
	UE_LOG(LogTemp, Warning, TEXT("Running Release %s"), *GetNameSafe(this));
}

void UCMagic::Reset(AActor* InstigatorActor)
{
	bIsPressing = false;

	if (AnimInstance->Montage_IsActive(Montage)) {
		StopMontage();
	}

	if(AnimInstance->OnPlayMontageNotifyBegin.IsBound())
	{
		AnimInstance->OnPlayMontageNotifyBegin.Remove(this, "OnNotifyBegin");	
	}

	UE_LOG(LogTemp, Warning, TEXT("Running Reset %s"), *GetNameSafe(this));
}

void UCMagic::MagicExecute(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("MagicExecute in Magic"));

	FVector Origin = InstigatorActor->GetActorLocation();
	FVector Start = Origin + 100.f * InstigatorActor->GetActorForwardVector();
	FVector End = Start + 2000.f * InstigatorActor->GetActorForwardVector();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorActor);
	FHitResult Hit;
	FColor DebugColorLocal;
	bool Success = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
	if (Success)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 32, FColor::Red, false, 3.0f);
		UCGameplayLibrary::ApplyDamage(InstigatorActor, Hit.GetActor());

	}
	DrawDebugLine(GetWorld(), Start, End, this->DebugMagicColor, false, 5.f, DebugLineThickness);
}

void UCMagic::StartMontage()
{
	if (AnimInstance) {

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCMagic::OnNotifyBegin);
		AnimInstance->Montage_Play(Montage, InPlayRate);

		if (!ensureMsgf(!MontageSection.IsNone(), TEXT("Magic must have montage Section Name assigned!"))) 
			return;

		AnimInstance->Montage_JumpToSection(MontageSection, Montage);
	}
}

void UCMagic::StopMontage()
{
	if (AnimInstance) {
		if (AnimInstance->OnPlayMontageNotifyBegin.Contains(this, "OnNotifyBegin"))
		{
			AnimInstance->OnPlayMontageNotifyBegin.Remove(this, "OnNotifyBegin");
		}
		AnimInstance->Montage_Stop(InBlendOutTime, Montage);
	}
}

bool UCMagic::IsMontagePlaying() const
{
	return AnimInstance->Montage_IsActive(Montage);
}

void UCMagic::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("OnNotifyBegin inside CMagic.cpp"));
	MagicExecute(BranchingPointPayload.SkelMeshComponent->GetOwner());
	float Cooldown = GetAnimMontageLength();
	if (Cooldown != -1)
		OnMagicExecuted.Broadcast(Cooldown);
}

UAnimInstance* UCMagic::GetAnimInstance() const
{
	return AnimInstance;
}


UCMagic::UCMagic()
{
	bIsPressing = false;
	InPlayRate = 1.0f;
	InBlendOutTime = 0.0f;
}

void UCMagic::Initialize(AActor* InstigatorActor)
{
	if (ensure(InstigatorActor))
	{
		UAnimInstance* Anim;
		ACharacter* CharacterTemp = Cast<ACharacter>(InstigatorActor);
		if (CharacterTemp)
		{
			Anim = CharacterTemp->GetMesh()->GetAnimInstance();

			if (ensure(Anim))
			{
				AnimInstance = Anim;
			}
		}
	}
}

bool UCMagic::IsPressing() const
{
	return bIsPressing;
}

float UCMagic::GetAnimMontageLength()
{
	if (Montage && AnimInstance->Montage_IsPlaying(Montage))
	{
		int32 SecIndex = Montage->GetSectionIndex(MontageSection);
		return Montage->GetSectionLength(SecIndex);
	}
	return -1.f;
}

FGameplayTagContainer UCMagic::GetGrantedTags() const
{
	return GrantedTags;
}

UWorld* UCMagic::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}