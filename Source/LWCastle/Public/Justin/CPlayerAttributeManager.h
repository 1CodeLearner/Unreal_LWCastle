// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPlayerAttributeManager.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class LWCASTLE_API UCPlayerAttributeManager : public UObject
{
	GENERATED_BODY()
public:
	UCPlayerAttributeManager();

protected:
	TObjectPtr<UDataTable> DT_PlayerHealth;
	TObjectPtr<UDataTable> DT_PlayerMana;
	TObjectPtr<UDataTable> DT_PlayerStamina;
};
