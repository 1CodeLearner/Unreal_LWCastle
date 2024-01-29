// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Magic/CMagic.h"
#include "Justin/CGameplayLibrary.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UCMagic::Press_Implementation(AActor* InstigatorActor)
{
	bIsPressing = true;
}

void UCMagic::Release_Implementation(AActor* InstigatorActor)
{
	bIsPressing = false;
}

void UCMagic::Reset(AActor* InstigatorActor, bool bIsPausing)
{
	if (!bIsPausing)
	{
		bIsPressing = false;
	}

	if (AnimInstance->Montage_IsActive(Montage)) {
		StopMontage();
	}

	if (AnimInstance->OnPlayMontageNotifyBegin.IsBound())
	{
		AnimInstance->OnPlayMontageNotifyBegin.Remove(this, "OnNotifyBegin");
	}
}

void UCMagic::MagicExecute_Implementation(AActor* InstigatorActor)
{
	if (AttributeComp)
	{
		FHitResult hitInfo;

		if (AttributeComp->TrySpendMana(ManaSpendAmount))
		{
			TArray<UActorComponent*> MeshComps = InstigatorActor->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), GunComponentTagName);
			USkeletalMeshComponent* GunMeshComp = nullptr;

			ensure(MeshComps.Num() == 1);
			for (auto MeshComp : MeshComps)
			{
				GunMeshComp = Cast<USkeletalMeshComponent>(MeshComp);
			}
			UCameraComponent* CameraComp = InstigatorActor->GetComponentByClass<UCameraComponent>();

			if (ensure(GunMeshComp) && ensure(CameraComp)) {
				FTransform fireposition = GunMeshComp->GetSocketTransform(TEXT("FirePosition2"));

				FVector startPos = CameraComp->GetComponentLocation() + FVector::OneVector * SweepRadius;
				FVector endPos = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * SweepDistanceFallback;

				FCollisionQueryParams params;
				params.AddIgnoredActor(InstigatorActor);
				bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
				if (bHit)
				{
					FTransform magicTrans;
					magicTrans.SetLocation(hitInfo.ImpactPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MagicEffectFactory, magicTrans);
				}
			}

		}
		else
		{
			TArray<UActorComponent*> MeshComps = InstigatorActor->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), GunComponentTagName);
			USkeletalMeshComponent* GunMeshComp = nullptr;

			ensure(MeshComps.Num() == 1);
			for (auto MeshComp : MeshComps)
			{
				GunMeshComp = Cast<USkeletalMeshComponent>(MeshComp);
			}
			UCameraComponent* CameraComp = InstigatorActor->GetComponentByClass<UCameraComponent>();

			if (ensure(GunMeshComp) && ensure(CameraComp)) {
				FTransform fireposition = GunMeshComp->GetSocketTransform(TEXT("FirePosition2"));

				FVector startPos = CameraComp->GetComponentLocation() + FVector::OneVector * SweepRadius;
				FVector endPos = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * SweepDistanceFallback;

				FCollisionQueryParams params;
				params.AddIgnoredActor(InstigatorActor);
				bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
				if (bHit)
				{
					DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 32.f, 24, FColor::Blue, false, 2.f);
				}
			}
			//Special effect for showing empty mana
			//DrawDebugCircle(GetWorld(), InstigatorActor->GetActorLocation(), 200.f, 12, FColor::Blue);
		}
	}
}

void UCMagic::StartMontage()
{
	if (AnimInstance) {
		//Making sure Anim Montage has notify available
		if (ensure(Montage->IsNotifyAvailable())) {
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCMagic::OnNotifyBegin);
			AnimInstance->Montage_Play(Montage, InPlayRate);

			if (!ensureMsgf(!MontageSection.IsNone(), TEXT("Magic must have montage Section Name assigned!")))
				return;

			UE_LOG(LogTemp, Warning, TEXT("Start Montage MagicFireAtRelease"));
			AnimInstance->Montage_JumpToSection(MontageSection, Montage);
		}
	}
}

void UCMagic::StopMontage()
{
	if (AnimInstance) {
		ClearNotifyBinds();
		AnimInstance->Montage_Stop(InBlendOutTime, Montage);
	}
}

void UCMagic::ClearNotifyBinds()
{
	if (AnimInstance->OnPlayMontageNotifyBegin.Contains(this, "OnNotifyBegin"))
	{
		AnimInstance->OnPlayMontageNotifyBegin.Remove(this, "OnNotifyBegin");
	}
}

bool UCMagic::IsMontagePlaying() const
{
	return AnimInstance->Montage_IsActive(Montage);
}

void UCMagic::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	MagicExecute(BranchingPointPayload.SkelMeshComponent->GetOwner());
	float Cooldown = GetAnimMontageLength();
	//Display Cooldown on UI
	if (Cooldown != -1.f)
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

void UCMagic::Initialize_Implementation(AActor* InstigatorActor)
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

		auto Attribute = InstigatorActor->GetComponentByClass<UCPlayerAttributeComp>();
		if (ensure(Attribute))
		{
			AttributeComp = Attribute;
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