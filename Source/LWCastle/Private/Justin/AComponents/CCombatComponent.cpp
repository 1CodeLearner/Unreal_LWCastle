// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/Actions/CAction_MagicAttack.h"
#include "Justin/Magic/CMagic.h"

UCCombatComponent::UCCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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
			FElementData* Data = DT_ElementData->FindRow<FElementData>(Name, TEXT(""));

			FElement ElementToAdd;
			UCMagic* Default = NewObject<UCMagic>(GetOwner(), Data->DefaultElement);
			UCMagic* Charged = NewObject<UCMagic>(GetOwner(), Data->ChargedElement);

			ElementToAdd.DefaultElement = Default;
			ElementToAdd.ChargedElement = Charged;
			OwningElements.Add(Name, ElementToAdd);
			Temp = Name;
		}
	}
	if (OwningElements.Num() > 0)
		ActiveElement = OwningElements[Temp];
}

void UCCombatComponent::SwitchElementByName(FName ElementName)
{
	if (!ensure(ElementName.IsNone())) {
		FElementData* DTData = DT_ElementData->FindRow<FElementData>(ElementName,TEXT(""));

		FElementData ElementDataTemp = *DTData;
		ActiveElement = GetElementFromName(ElementName);

		OnActiveElementSwitched.Broadcast(GetOwner(), ElementDataTemp, ActiveElement);
	}
}

FElement UCCombatComponent::GetElementFromName(FName Name) const
{
	return OwningElements[Name];
}

