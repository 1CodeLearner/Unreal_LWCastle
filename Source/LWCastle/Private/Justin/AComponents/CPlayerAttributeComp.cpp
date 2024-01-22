// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "kismet/GameplayStatics.h"
#include "Justin/CGameModeBase.h"

UCPlayerAttributeComp::UCPlayerAttributeComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCPlayerAttributeComp::BeginPlay()
{
	Super::BeginPlay();
	auto PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		auto Manager = PC->GetComponentByClass<UCPlayerAttributeManagerComp>();
		if (Manager)
		{
			Manager->OnPlayerStatUpdated.AddDynamic(this, &UCPlayerAttributeComp::OnStatUpdated);
			auto StatsInfo = Manager->GetAllStats();
			MaxHealth = StatsInfo.MaxHealth;
			MaxMana = StatsInfo.MaxMana;
			MaxStamina = StatsInfo.MaxStamina;
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not found!"));
	}

	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
	CurrentStamina = MaxStamina;
	RecoverToFull();
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

void UCPlayerAttributeComp::RecoverToFull()
{
	CurrentMana = MaxMana;
	CurrentStamina = MaxStamina;
	CurrentHealth = MaxHealth;
	OnAttributeChange.Broadcast(EPlayerStat::HEALTH, CurrentHealth, MaxHealth);
	OnAttributeChange.Broadcast(EPlayerStat::MANA, CurrentMana, MaxMana);
	OnAttributeChange.Broadcast(EPlayerStat::STAMINA, CurrentStamina, MaxStamina);
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

void UCPlayerAttributeComp::SpendMana(float SpendAmount)
{
	CurrentMana -= SpendAmount;
	OnAttributeChange.Broadcast(EPlayerStat::MANA, CurrentMana, MaxMana);
	if (CurrentMana <= 0)
	{
		CurrentMana = 0;
		OnManaDepleted.Broadcast();
	}

}

void UCPlayerAttributeComp::SpendStamina(float SpendAmount)
{
	if (CurrentStamina > 0.f)
	{
		CurrentStamina -= SpendAmount;
		if (CurrentStamina < 0.f)
			CurrentStamina = 0.f;
		OnAttributeChange.Broadcast(EPlayerStat::STAMINA, CurrentStamina, MaxStamina);
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
	ACGameModeBase* GM = Cast<ACGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{

		FStruct_Progression Progression = GM->GetCurrentProgressionOf(StatInfo.PlayerStatEnum, StatInfo.Level);
		switch (StatInfo.PlayerStatEnum)
		{
		case EPlayerStat::HEALTH:
		{
			MaxHealth = Progression.Amount;
			CurrentHealth = MaxHealth;
			OnAttributeChange.Broadcast(EPlayerStat::HEALTH, CurrentHealth, MaxHealth);
			break;
		}
		case EPlayerStat::MANA:
		{
			MaxMana = Progression.Amount;
			CurrentMana = MaxMana;
			OnAttributeChange.Broadcast(EPlayerStat::MANA, CurrentMana, MaxMana);
			break;
		}
		case EPlayerStat::STAMINA:
		{
			MaxStamina = Progression.Amount;
			CurrentStamina = MaxStamina;
			OnAttributeChange.Broadcast(EPlayerStat::STAMINA, CurrentStamina, MaxStamina);
			break;
		}
		}

		//OnProgressionChanged.Broadcast(StatInfo.PlayerStatEnum, Progression.Amount);
	}
}

