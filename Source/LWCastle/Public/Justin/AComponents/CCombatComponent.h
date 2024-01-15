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

	TObjectPtr<UCAction_MagicAttack> ActiveDefaultMagic;
	TObjectPtr<UCAction_MagicAttack> ActiveChargeMagic;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActiveMagicSwitchedDelegate, AActor*, InstigatorActor, FMagicAttackGroup, ActiveMagicGroup);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCombatComponent();
	UPROPERTY(BlueprintCallable, Category = "Combat")
	FActiveMagicSwitchedDelegate OnActiveMagicSwitched;

	UFUNCTION(BlueprintCallable)
	FMagicAttackGroup GetActiveMagic() const;

protected:

	//Magic attack that is Active
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UCAction_MagicAttack> ActiveDefaultMagic;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UCAction_MagicAttack> ActiveChargedMagic;
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<UCAction>> OwningDefaultMagicClasses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<UCAction>> OwningChargedMagicClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<UCAction_MagicAttack*> OwningDefaultMagic;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<UCAction_MagicAttack*> OwningChargedMagic;

protected:
	virtual void BeginPlay() override;


};
