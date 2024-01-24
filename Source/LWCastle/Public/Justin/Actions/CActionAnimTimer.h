// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionPause.h"
#include "CActionAnimTimer.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	START UMETA(DisplayName = "START"),
	PAUSE UMETA(DisplayName = "PAUSE"),
	UNPAUSE UMETA(DisplayName = "UNPAUSE"),
	INTERRUPT UMETA(DisplayName = "INTERRUPT"),
	COMPLETE UMETA(DisplayName = "COMPLETE"),
	NONE UMETA(DisplayName = "NONE")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FActionInvokedDelegate, EActionType, ActionType, AActor*, InstigatorActor, float, Duration);

/**
 *
 */
UCLASS(Abstract)
class LWCASTLE_API UCActionAnimTimer : public UCActionPause//, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;

	UPROPERTY(BlueprintAssignable, Category = "ActionAnimTimer")
	FActionInvokedDelegate OnActionInvoked;


protected:
	//Animation Montage
	virtual void StartMontage(UCActionAnimTimer* ActionAnimTimer);
	void StopMontage(UCActionAnimTimer* ActionAnimTimer);
	void UnbindNotifyEvent(UCActionAnimTimer* AnimTimer);
	UFUNCTION(BlueprintCallable)
	float GetAnimMontageLength();
	UFUNCTION()
	UAnimInstance* GetAnimInstance() const;
	bool IsMontagePlaying() const;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionAnimTimer")
	UAnimMontage* Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionAnimTimer")
	FName MontageSection;

	UFUNCTION()
	virtual void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

protected:
	//Timer
	virtual void StartTimer(UCActionAnimTimer* ActionAnimTimer);
	virtual void PauseTimer();
	virtual void UnPauseTimer();
	virtual void ClearTimer();
	bool IsTimerValid();
	float GetTimerDuration();
	float GetTimerRemaining();

	UFUNCTION(BlueprintCallable, Category = "ActionAnimTimer")
	virtual void ExecuteAction(AActor* InstigatorActor);

public:
	UCActionAnimTimer();

private:
	//Animation settings
	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0."), Category = "AnimSetting")
	float InBlendOutTime;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0."), Category = "AnimSetting")
	float InPlayRate;

	//Timer Settings
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"), Category = "TimerSetting")
	float TimerDuration;
};
