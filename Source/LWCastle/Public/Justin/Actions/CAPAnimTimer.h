// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Actions/CActionPause.h"
#include "CAPAnimTimer.generated.h"


/*UENUM(BlueprintType)
enum class EActionType
{
	START,
	PAUSE,
	UNPAUSE,
	INTERRUPT,
	COMPLETE,
	NONE
};*/

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FActionInvokedDelegate, EActionType, ActionType, AActor*, InstigatorActor, float, Duration);

/**
 *
 */
UCLASS(Abstract)
class LWCASTLE_API UCAPAnimTimer : public UCActionPause
{
	GENERATED_BODY()

public:

	virtual void Initialize_Implementation(UCGameplayComponent* GameplayComp) override;

	//UPROPERTY(BlueprintAssignable, Category = "ActionAnimTimer")
	//FActionInvokedDelegate OnActionInvoked;


protected:
	//Animation Montage
	virtual void StartMontage(UCAPAnimTimer* AnimTimer);
	void StopMontage(UCAPAnimTimer* AnimTimer);
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
	virtual void StartTimer(UCAPAnimTimer* AnimTimer);
	virtual void PauseTimer();
	virtual void UnPauseTimer();
	virtual void ClearTimer();
	bool IsTimerValid();

	UFUNCTION()
	virtual void ExecuteAction(AActor* InstigatorActor);

public:
	UCAPAnimTimer();

private:
	//Animation settings
	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0."), Category = "AnimSetting")
	float InBlendOutTime;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.1"), Category = "AnimSetting")
	float InPlayRate;

	//Timer Settings
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"), Category = "TimerSetting")
	float TimerDuration;
};
