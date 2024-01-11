// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "kismet/GameplayStatics.h"
#include "Justin/CGameModeBase.h"

UCPlayerAttributeComp::UCPlayerAttributeComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxMana = 100;
	CurrentMana = MaxMana;
	MaxStamina = 80;
	CurrentMana = MaxStamina;
	StaminaSpendRate = .5;
}

void UCPlayerAttributeComp::UpdateStats()
{
	/*ACGameModeBase* PlayerGM = Cast<ACGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (PlayerGM)
	{
		UDataTable* DT_Progress = PlayerGM->GetProgressionTableOf(PlayerGM->);

		TArray<FStruct_Progression*> Rows;
		FString str("Context");
		DT_Progress->GetAllRows<FStruct_Progression>(*str, Rows);

		float UpdatedValue = 0.f;
		for (auto Attribute : Rows)
		{
			if (Attribute->Level == StatInfo.Level)
				UpdatedValue = Attribute->Amount;
		}

		switch (StatInfo.PlayerStatEnum)
		{
		case EPlayerStat::HEALTH:
		{
			MaxHealth += UpdatedValue;
			break;
		}
		case EPlayerStat::MANA:
		{
			MaxMana += UpdatedValue;
			break;
		}
		case EPlayerStat::STAMINA:
		{
			MaxStamina += UpdatedValue;
			break;
		}
		}
		OnProgressionChanged.Broadcast(StatInfo.PlayerStatEnum, UpdatedValue);
	}*/


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

FStruct_StatDisplays UCPlayerAttributeComp::GetAttributesToDisplay() const
{
	return { CurrentHealth, MaxHealth, CurrentMana, MaxMana, CurrentStamina, MaxStamina };
}

float UCPlayerAttributeComp::GetCurrentMana() const
{
	return CurrentMana;
}

float UCPlayerAttributeComp::GetCurrentStamina() const
{
	return CurrentStamina;
}


float UCPlayerAttributeComp::GetMaxStamina() const
{
	return MaxStamina;
}

float UCPlayerAttributeComp::GetMaxMana() const
{
	return MaxMana;
}

bool UCPlayerAttributeComp::TrySpendMana(int SpendAmount)
{
	if (CurrentMana <= SpendAmount)
	{
		return false;
	}
	else
	{
		CurrentMana -= SpendAmount;
		if (CurrentMana <= 0)
		{
			CurrentMana = 0;
			OnManaDepleted.Broadcast();
		}
		return true;
	}
}

void UCPlayerAttributeComp::SpendStamina(float SpendAmount)
{
	if (CurrentStamina > 0.f)
	{
		CurrentStamina -= SpendAmount;
	}
	else if (CurrentStamina <= 0.f)
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

}

