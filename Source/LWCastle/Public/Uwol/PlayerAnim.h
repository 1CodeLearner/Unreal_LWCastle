// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Uwol/uwol_test.h"
#include "PlayerAnim.generated.h"

/**
 *
 */
UCLASS()
class LWCASTLE_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float speed = 0;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 공중 체크
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool isInAir = false;

	// 좌우 이동속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float verdirection = 0;

	// 공격 재생 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* attackAnimMontage;

	// 애니메이션 재생
	void PlayAttackAnim();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool checkdodge = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool checkattack = false;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerAnim")
	bool bIsAiming = false;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerAnim")
	bool bIsDead = false;
};
