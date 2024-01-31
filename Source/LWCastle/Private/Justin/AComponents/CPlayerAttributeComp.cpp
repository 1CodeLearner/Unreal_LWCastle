// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/AComponents/CPlayerAttributeManagerComp.h"
#include "kismet/GameplayStatics.h"
#include "Justin/CGameModeBase.h"

UCPlayerAttributeComp::UCPlayerAttributeComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsManaRecovering = false;
	bIsStaminaRecovering = false;
	bIsChannelManaRecovering = false;
	bIsChannelingMana = false;
	bIsChannelingManaDepleted = false;
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

	SetComponentTickEnabled(false);
}

void UCPlayerAttributeComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	if (bIsManaRecovering)
	{
		CurrentMana += ManaRecoveryRate * DeltaTime;
		if (CurrentMana >= MaxMana)
		{
			CurrentMana = MaxMana;
			bIsManaRecovering = false;
		}
	}
	if (bIsChannelManaRecovering)
	{
		ChanneledManaAmount += ManaRecoveryRate * DeltaTime;
	}

	if (bIsManaRecovering || bIsChannelManaRecovering)
	{
		DisplayStats(EPlayerStat::MANA);
	}

	if (bIsStaminaRecovering)
	{
		CurrentStamina += StaminaRecoveryRate * DeltaTime;
		if (CurrentStamina >= MaxStamina)
		{
			CurrentStamina = MaxStamina;
			bIsStaminaRecovering = false;
		}
		DisplayStats(EPlayerStat::STAMINA);
	}

	CheckAndDisableTick();

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

bool UCPlayerAttributeComp::TryChannelMana(float Value)
{
	if (!bIsChannelingMana)
	{
		bIsChannelingMana = true;
		ChanneledManaAmount = CurrentMana;
	}

	if (bIsChannelingManaDepleted)
		return false;

	ChanneledManaAmount -= Value;

	bIsChannelManaRecovering = false;
	GetWorld()->GetTimerManager().ClearTimer(ChannelManaRecoveryHandle);

	if (ChanneledManaAmount <= 0)
	{
		ChanneledManaAmount = 0;
		bIsChannelingManaDepleted = true;

		ChannelManaRecoveryDelegate.BindUFunction(this, "EnableChannelManaRecovery", true);
		GetWorld()->GetTimerManager().SetTimer(ChannelManaRecoveryHandle, ChannelManaRecoveryDelegate, 0.001f, false, ChannelManaRecoveryDelay);
		DisplayStats(EPlayerStat::MANA);

		return false;
	}

	DisplayStats(EPlayerStat::MANA);
	return true;
}

void UCPlayerAttributeComp::CancelChannelingMana()
{
	if (bIsChannelingMana)
	{
		ChanneledManaAmount = 0.f;

		bIsChannelingMana = false;
		bIsChannelingManaDepleted = false;
		DisplayStats(EPlayerStat::MANA);
		GetWorld()->GetTimerManager().ClearTimer(ChannelManaRecoveryHandle);
		GetWorld()->GetTimerManager().ClearTimer(ChannelManaDepletedHandle);
	}
}

void UCPlayerAttributeComp::CompleteChannelingMana()
{
	if (bIsChannelingMana)
	{
		CurrentMana = ChanneledManaAmount;
		if (CurrentMana < MaxMana)
			StartManaRecoveryCooldown();

		bIsChannelingMana = false;
		bIsChannelingManaDepleted = false;
		DisplayStats(EPlayerStat::MANA);
		GetWorld()->GetTimerManager().ClearTimer(ChannelManaRecoveryHandle);
		GetWorld()->GetTimerManager().ClearTimer(ChannelManaDepletedHandle);
	}
}

float UCPlayerAttributeComp::GetMaxMana() const
{
	return MaxMana;
}

bool UCPlayerAttributeComp::TrySpendMana(float SpendAmount)
{
	if (bIsChannelingMana)
		CancelChannelingMana();

	if (CurrentMana - SpendAmount < 0)
		return false;

	CurrentMana -= SpendAmount;

	StartManaRecoveryCooldown();

	if (CurrentMana <= 0)
	{
		CurrentMana = 0;
		OnManaDepleted.Broadcast();
	}

	DisplayStats(EPlayerStat::MANA);

	return true;
}

bool UCPlayerAttributeComp::SpendStamina(float SpendAmount)
{
	if (CurrentStamina > 0.f)
	{
		CurrentStamina -= SpendAmount;

		bIsStaminaRecovering = false;
		GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryCooldownHandle);
		StaminaRecoveryDelegate.BindUFunction(this, "EnableStaminaRecovery", true);
		GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryCooldownHandle, StaminaRecoveryDelegate, 0.001f, false, StaminaRecoveryDelay);

		if (CurrentStamina < 0.f)
			CurrentStamina = 0.f;
	}
	else if (CurrentStamina <= 0.f)
	{
		OnStaminaDepleted.Broadcast();
		return false;
	}

	DisplayStats(EPlayerStat::STAMINA);
	return true;
}

void UCPlayerAttributeComp::RecoverMana(float Value)
{
	if (bIsChannelingMana)
	{
		ChanneledManaAmount += Value;
		if (ChanneledManaAmount >= MaxMana)
		{
			ChanneledManaAmount = MaxMana;
			OnManaFullReached.Broadcast();
		}
	}
	else
	{
		CurrentMana += Value;
		if(CurrentMana >= MaxMana)
		{
			CurrentMana = MaxMana;
		}
	}

	DisplayStats(EPlayerStat::MANA);
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
			DisplayStats(EPlayerStat::HEALTH);
			break;
		}
		case EPlayerStat::MANA:
		{
			MaxMana = Progression.Amount;
			CurrentMana = MaxMana;
			DisplayStats(EPlayerStat::MANA);
			break;
		}
		case EPlayerStat::STAMINA:
		{
			MaxStamina = Progression.Amount;
			CurrentStamina = MaxStamina;
			DisplayStats(EPlayerStat::STAMINA);
			break;
		}
		}

		//OnProgressionChanged.Broadcast(StatInfo.PlayerStatEnum, Progression.Amount);
	}
}

void UCPlayerAttributeComp::DisplayStats(EPlayerStat StatType)
{
	switch (StatType)
	{
	case EPlayerStat::MANA:
	{
		if (bIsChannelingMana)
			OnAttributeChange.Broadcast(StatType, ChanneledManaAmount, MaxMana);
		else
			OnAttributeChange.Broadcast(StatType, CurrentMana, MaxMana);
		break;
	}
	case EPlayerStat::HEALTH:
	{
		OnAttributeChange.Broadcast(StatType, CurrentHealth, MaxHealth);
		break;
	}
	case EPlayerStat::STAMINA:
		OnAttributeChange.Broadcast(StatType, CurrentStamina, MaxStamina);
		break;
	}
}

void UCPlayerAttributeComp::StartManaRecoveryCooldown()
{
	bIsManaRecovering = false;
	GetWorld()->GetTimerManager().ClearTimer(ManaRecoveryCooldownHandle);
	ManaRecoveryDelegate.BindUFunction(this, "EnableManaRecovery", true);
	GetWorld()->GetTimerManager().SetTimer(ManaRecoveryCooldownHandle, ManaRecoveryDelegate, 0.001f, false, ManaRecoveryDelay);
}

void UCPlayerAttributeComp::CheckAndDisableTick()
{
	if (bIsManaRecovering || bIsStaminaRecovering)
		return;
	else
		SetComponentTickEnabled(false);
}

void UCPlayerAttributeComp::EnableManaRecovery(bool bEnabled)
{
	bIsManaRecovering = bEnabled;
	if (bIsManaRecovering && !IsComponentTickEnabled())
	{
		SetComponentTickEnabled(true);
	}
}

void UCPlayerAttributeComp::EnableStaminaRecovery(bool bEnabled)
{
	bIsStaminaRecovering = bEnabled;
	if (bIsStaminaRecovering && !IsComponentTickEnabled())
	{
		SetComponentTickEnabled(true);
	}
}

void UCPlayerAttributeComp::EnableChannelManaRecovery(bool bEnabled)
{
	bIsChannelManaRecovering = bEnabled;

	if (bIsChannelManaRecovering && !IsComponentTickEnabled())
	{
		SetComponentTickEnabled(true);
	}

	if (bIsChannelingManaDepleted)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChannelManaDepletedHandle);
		GetWorld()->GetTimerManager().SetTimer(ChannelManaDepletedHandle, this, &UCPlayerAttributeComp::ChannelManaDepletedReset, 0.001f, false, ChannelManaDepletedDelay);
	}
}

void UCPlayerAttributeComp::ChannelManaDepletedReset()
{
	bIsChannelingManaDepleted = false;
}
