// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "kismet/GameplayStatics.h"

UCPlayerAttributeComp::UCPlayerAttributeComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	ManaPoints = 100;
	Stamina = 80;
	StaminaSpendRate = .5;
}

void UCPlayerAttributeComp::BeginPlay()
{
	Super::BeginPlay();
	auto PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		auto ActorComponent = PC->GetComponentByClass<UCPlayerAttributeManagerComp>();
		if (ActorComponent)
		{
			ActorComponent->OnPlayerStatUpdated.AddDynamic(this, &UCPlayerAttributeComp::OnStatUpdated);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not found!"));
	}
}

void UCPlayerAttributeComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if input is enabled,
		//enable tick and start draining Stamina.
		//if stamina is depleted.
			//send broadcast and stop tick.
}

int UCPlayerAttributeComp::GetManaPoints() const
{
	return ManaPoints;
}

float UCPlayerAttributeComp::GetStamina() const
{
	return Stamina;
}


bool UCPlayerAttributeComp::TrySpendMana(int SpendAmount)
{
	if (ManaPoints <= SpendAmount)
	{
		return false;
	}
	else
	{
		ManaPoints -= SpendAmount;
		if (ManaPoints <= 0)
		{
			ManaPoints = 0;
			OnManaDepleted.Broadcast();
		}
		return true;
	}
}

void UCPlayerAttributeComp::SpendStamina(float SpendAmount)
{
	if (Stamina > 0.f)
	{
		Stamina -= SpendAmount;
	}
	else if (Stamina <= 0.f)
	{
		OnStaminaDepleted.Broadcast();
	}
}

void UCPlayerAttributeComp::EnableSpendingStaminaByRate(bool bIsEnabled)
{
	SetComponentTickEnabled(bIsEnabled);
}

//Bring Player progression info from GameMode.
void UCPlayerAttributeComp::OnStatUpdated(FStatInfo StatInfo)
{
	switch (StatInfo.PlayerStatEnum)
	{
	case EPlayerStat::HEALTH:
	{
		MaxHealth += 0;
		break;
	}
	case EPlayerStat::MANA:
	{
		ManaPoints += 0;
		break;
	}
	case EPlayerStat::STAMINA:
	{
		Stamina += 0;
		break;
	}
	}

}

