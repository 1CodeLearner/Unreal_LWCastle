// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCombatComponent.generated.h"


class UCAction;
class UCAction_MagicAttack;

USTRUCT(BlueprintType)
struct FMagicAttackGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCAction_MagicAttack> ActiveDefaultMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCAction_MagicAttack> ActiveChargeMagic;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActiveMagicSwitchedDelegate, AActor*, InstigatorActor, FMagicAttackGroup, ActiveMagicGroup);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeStateActivatedDelegate, bool, bIsCharged);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCombatComponent();
	
	UPROPERTY(BlueprintCallable, Category = "Combat")
	FActiveMagicSwitchedDelegate OnActiveMagicSwitched;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat")
	FChargeStateActivatedDelegate OnChargeStateActivated;

	UFUNCTION(BlueprintCallable)
	FMagicAttackGroup GetActiveMagic() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Initialize();

protected:

	//Magic attack that is Active
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UCAction_MagicAttack> ActiveDefaultMagic;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UCAction_MagicAttack> ActiveChargedMagic;
	//

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TArray<UCAction_MagicAttack*> OwningDefaultMagic;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TArray<UCAction_MagicAttack*> OwningChargedMagic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<UCAction>> OwningDefaultMagicClasses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<UCAction>> OwningChargedMagicClasses;
};
