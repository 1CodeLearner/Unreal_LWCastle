// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Justin/CStateInterface.h"
#include "CItemBase.generated.h"

/**
 * 
 */
class UWorld;
UCLASS(Blueprintable)
class LWCASTLE_API UCItemBase : public UObject, public ICStateInterface
{
	GENERATED_BODY()
public:
	void Start_Implementation(AActor* InstigatorActor);
	void Stop_Implementation(AActor* InstigatorActor);
	void Pause_Implementation(AActor* InstigatorActor);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TagName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Cost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> SMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPersistent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> DisplayTexture;
protected:
	virtual UWorld* GetWorld() const override;
};
