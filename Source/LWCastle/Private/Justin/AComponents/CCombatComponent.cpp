// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CCombatComponent.h"

#include "Justin/CPlayerController.h"
#include "Justin/Magic/CMagic.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/Widgets/CChargeWidget.h"

UCCombatComponent::UCCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	AccumulatedMana = 0.f;
	bIsChannelManaActive = false;
}

FElement UCCombatComponent::GetActiveElement() const
{
	return ActiveElement;
}

void UCCombatComponent::Initialize()
{
	ensure(OwningElements.Num() == 0);
	FName Temp;
	if (ensure(DT_ElementData)) {
		TArray<FName> Names = DT_ElementData->GetRowNames();
		for (auto Name : Names)
		{
			FElementData* ElementDataToAdd = DT_ElementData->FindRow<FElementData>(Name, TEXT(""));

			FElement ElementToAdd;

			UCMagic* Default = NewObject<UCMagic>(GetOwner(), ElementDataToAdd->DefaultElement);
			Default->Initialize(GetOwner());
			UCMagic* Charged = NewObject<UCMagic>(GetOwner(), ElementDataToAdd->ChargedElement);
			Charged->Initialize(GetOwner());
			Charged->OnMagicExecuted.AddDynamic(this, &UCCombatComponent::OnChargeMagicExecuted);

			ElementToAdd.ElementName = Name;
			ElementToAdd.DefaultElement = Default;
			ElementToAdd.ChargedElement = Charged;
			OwningElements.Add(Name, ElementToAdd);

			FElementData ElementData = *ElementDataToAdd;
			OwningElementData.Add(Name, ElementData);

			Temp = Name;
		}
	}
	if (OwningElements.Num() > 0)
		ActiveElement = OwningElements[Temp];

	auto GameplayTemp = GetOwner()->GetComponentByClass<UCGameplayComponent>();
	if (ensure(GameplayTemp))
	{
		GameplayComp = GameplayTemp;
	}
}

void UCCombatComponent::InitializeWidget()
{
	if (!Widget)
	{
		auto PlayerController = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController) {
			if (!PlayerController->ChargeWidget)
			{
				PlayerController->Initialize();
			}
			Widget = PlayerController->ChargeWidget;
		}
	}
}

void UCCombatComponent::SwitchElementByName(FName ElementName)
{
	if (ensure(!ElementName.IsNone())) {
		FElementData* DTData = DT_ElementData->FindRow<FElementData>(ElementName, TEXT(""));

		FElementData ElementDataTemp = *DTData;
		ActiveElement = GetElementFromName(ElementName);

		OnActiveElementSwitched.Broadcast(GetOwner(), ElementDataTemp, ActiveElement);
	}
}

FElementData UCCombatComponent::GetActiveElementData() const
{
	return OwningElementData[ActiveElement.ElementName];
}

FElement UCCombatComponent::GetElementFromName(FName Name) const
{
	return OwningElements[Name];
}

void UCCombatComponent::StartChannelMana()
{
	Widget->SetVisibilityWidget(ESlateVisibility::Visible);
	bIsChannelManaActive = true;
}

void UCCombatComponent::AddChannelMana(float Value)
{
	if (bIsChannelManaActive)
	{
		AccumulatedMana += Value;

		Widget->UpdateWidget(GetActiveElementData().ChargeManaTotal, AccumulatedMana);
		if (AccumulatedMana >= GetActiveElementData().ChargeManaTotal)
		{
			AccumulatedMana = GetActiveElementData().ChargeManaTotal;
			Widget->UpdateWidget(GetActiveElementData().ChargeManaTotal, AccumulatedMana);
			AccumulatedMana = 0.f;
			if (OnManaCharged.IsBound())
			{
				OnManaCharged.Execute(GetOwner());
			}
			bIsChannelManaActive = false;
		}
	}
}

void UCCombatComponent::ResetChannelMana()
{
	AccumulatedMana = 0.f;
	Widget->ResetWidget();
	bIsChannelManaActive = false;
}

void UCCombatComponent::CompleteChannelMana()
{
	AccumulatedMana = 0.f;
	bIsChannelManaActive = false;
}

void UCCombatComponent::OnChargeMagicExecuted(float CooldownLength)
{
	if (GameplayComp)
	{
		static FGameplayTag ChargedStateTag = FGameplayTag::RequestGameplayTag("State.Charged");
		if (GameplayComp->ActiveGameplayTags.HasTagExact(ChargedStateTag))
		{
			GameplayComp->CompleteActionByName(GetOwner(), "ChargedState");
		}
	}
}

