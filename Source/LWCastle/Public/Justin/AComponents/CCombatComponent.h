// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCombatComponent.generated.h"


class UCAction;
class UCMagic;
class UCGameplayComponent;

USTRUCT(BlueprintType)
struct FElement
{
	GENERATED_BODY()
public:
	
	FName ElementName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCMagic> DefaultElement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCMagic> ChargedElement;
};

USTRUCT(BlueprintType)
struct FElementData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCMagic> DefaultElement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCMagic> ChargedElement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChargeManaTotal;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FActiveElementSwitchedDelegate, AActor*, InstigatorActor, FElementData, ElementData, FElement, ActiveElement);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChargeStateActivatedDelegate, AActor*, InstigatorActor, bool, bIsCharged);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCombatComponent();

	UPROPERTY(BlueprintCallable, Category = "Combat")
	FActiveElementSwitchedDelegate OnActiveElementSwitched;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat")
	FChargeStateActivatedDelegate OnChargeStateActivated;

	UFUNCTION(BlueprintCallable)
	FElement GetActiveElement() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void SwitchElementByName(FName ElementName);

	UFUNCTION()
	FElementData GetActiveElementData() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	FElement ActiveElement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TMap<FName, FElement> OwningElements;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TMap<FName, FElementData> OwningElementData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UDataTable* DT_ElementData;

	UFUNCTION()
	FElement GetElementFromName(FName Name) const;

private:
	UFUNCTION()
	void OnChargeMagicExecuted(float CooldownLength);
	UPROPERTY()
	TObjectPtr<UCGameplayComponent> GameplayComp;
};
