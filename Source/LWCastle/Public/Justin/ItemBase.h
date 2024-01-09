// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LWCASTLE_API UItemBase : public UObject
{
	GENERATED_BODY()
public:
	UItemBase();
	FText DisplayName;
	FName ItemName;
	float Value;
	int Cost;
	UStaticMesh Mesh;
	bool bIsPersistent;
	UTexture2D DisplayTexture;

	UFUNCTION(BlueprintNativeEvent)
	void Activate();

	UFUNCTION(BlueprintNativeEvent)
	void DeActivate();

	UFUNCTION(BlueprintNativeEvent)
	void Execute();

protected:
	virtual UWorld* GetWorld() const override;
};
