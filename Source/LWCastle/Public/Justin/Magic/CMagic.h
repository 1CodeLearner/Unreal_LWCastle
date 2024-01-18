// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMagic.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class LWCASTLE_API UCMagic : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Magic")
	void Press(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Magic")
	void Release(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Magic")
	void Reset(AActor* InstigatorActor);
	UFUNCTION(BlueprintCallable)
	bool IsPressing() const;
	UFUNCTION(BlueprintCallable)
	float GetDelayTime() const;

protected:
	UFUNCTION()
	virtual void MagicExecute(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	FColor DebugMagicColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	float DebugLineThickness;
	FTimerHandle MagicHandle;
	FTimerDelegate MagicDelegate;

private:
	UPROPERTY(EditDefaultsOnly,meta = (AllowPrivateAccess="true", ClampMin="0."), Category = "Magic")
	float DelayTime;
	bool bIsPressing;
public:
	UCMagic();

protected:
	virtual UWorld* GetWorld() const override;
};
