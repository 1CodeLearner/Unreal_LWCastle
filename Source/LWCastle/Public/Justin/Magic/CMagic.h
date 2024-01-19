// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CMagic.generated.h"

/**
 *
 */

 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMagicExecuted, float, CooldownLength);

UCLASS(BlueprintType, Blueprintable)
class LWCASTLE_API UCMagic : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Magic")
	void Initialize(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Magic")
	void Press(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Magic")
	void Release(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Magic")
	void Reset(AActor* InstigatorActor);
	UFUNCTION(BlueprintCallable)
	bool IsPressing() const;
	UFUNCTION(BlueprintCallable)
	float GetAnimMontageLength();

	FMagicExecuted OnMagicExecuted;

	FGameplayTagContainer GetGrantedTags() const;

protected:
	UFUNCTION()
	virtual void MagicExecute(AActor* InstigatorActor);

	virtual void StartMontage();
	void StopMontage();
	bool IsMontagePlaying() const;

	UFUNCTION()
	virtual void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	UAnimMontage* Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	FName MontageSection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	FColor DebugMagicColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	float DebugLineThickness;
	
	UFUNCTION()
	UAnimInstance* GetAnimInstance() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	FGameplayTagContainer GrantedTags;

private:

	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0."), Category = "Magic")
	float InBlendOutTime;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.1"), Category = "Magic")
	float InPlayRate;

	bool bIsPressing;

public:
	UCMagic();

protected:
	virtual UWorld* GetWorld() const override;
};
