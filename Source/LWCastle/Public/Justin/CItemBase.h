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
	UPROPERTY(EditAnywhere)
	FText DisplayName;
	UPROPERTY(EditAnywhere)
	FName TagName;
	UPROPERTY(EditAnywhere)
	float Value;
	UPROPERTY(EditAnywhere)
	int Cost;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> SMesh;
	UPROPERTY(EditAnywhere)
	bool bIsPersistent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> DisplayTexture;
protected:
	virtual UWorld* GetWorld() const override;
};
