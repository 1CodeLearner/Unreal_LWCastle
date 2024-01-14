// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCombatComponent.generated.h"


class UCAction;
class UCAction_MagicAttack;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActiveMagicSwitchedDelegate, AActor*, InstigatorActor, UCAction_MagicAttack*, SwitchedMagicAttack);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LWCASTLE_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCombatComponent();
	UPROPERTY(BlueprintCallable)
	FActiveMagicSwitchedDelegate OnActiveMagicSwitched;

	UFUNCTION(BlueprintCallable)
	UCAction_MagicAttack* GetActiveMagic() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UCAction_MagicAttack> ActiveMagic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<UCAction>> OwningMagicClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<UCAction_MagicAttack*> OwningMagic;

protected:
	virtual void BeginPlay() override;


};
