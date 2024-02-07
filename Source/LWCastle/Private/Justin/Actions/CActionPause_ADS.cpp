// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Actions/CActionPause_ADS.h"
#include "Uwol/uwol_test.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

void UCActionPause_ADS::Initialize_Implementation(UCGameplayComponent* Component)
{
	Super::Initialize_Implementation(Component);
	Character = Cast<Auwol_test>(Component->GetOwner());
	ensure(Character);
}

void UCActionPause_ADS::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Character)
	{
		Character->bSniperAim = true;
		Character->_FocusUI->AddToViewport();
		Character->tpsCamComp->SetFieldOfView(45.0f);
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void UCActionPause_ADS::PauseAction_Implementation(AActor* Instigator)
{
	Super::PauseAction_Implementation(Instigator);
	Character->bSniperAim = false;
	Character->_FocusUI->RemoveFromParent();
	Character->tpsCamComp->SetFieldOfView(90.0f);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;

}

void UCActionPause_ADS::UnPauseAction_Implementation(AActor* Instigator)
{
	Super::UnPauseAction_Implementation(Instigator);
	Character->bSniperAim = true;
	Character->_FocusUI->AddToViewport();
	Character->tpsCamComp->SetFieldOfView(45.0f);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UCActionPause_ADS::CompleteAction_Implementation(AActor* Instigator)
{
	Super::CompleteAction_Implementation(Instigator);
	Character->bSniperAim = false;
	Character->_FocusUI->RemoveFromParent();
	Character->tpsCamComp->SetFieldOfView(90.0f);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UCActionPause_ADS::InterruptAction_Implementation(AActor* InstigatorActor)
{
	Super::InterruptAction_Implementation(InstigatorActor);

	Character->bSniperAim = false;
	Character->_FocusUI->RemoveFromParent();
	Character->tpsCamComp->SetFieldOfView(90.0f);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}



