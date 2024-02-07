// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/CGameModeBase.h"
#include "Justin/CItemBase.h"
#include "Justin/CGameplayLibrary.h"
#include "Engine/TriggerBox.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/Magic/CMagicProjectile.h"
#include "Uwol/uwol_test.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Components/BoxComponent.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"
#include "Justin/CPlayerController.h"
#include "Justin/EnemyCharacter.h"
#include "CAIController.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Justin/UserWidget_Gameplay.h"
#include "Justin/UserWidget_BossHealth.h"
#include "GameFramework/PlayerStart.h"

void ACGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (auto ItemBaseClass : ItemBaseClasses)
	{
		if (ItemBaseClass)
		{
			//UCItemBase* tempItem = NewObject<UCItemBase>(this, ItemBaseClass);
			UCItemBase* tempItem = ItemBaseClass.GetDefaultObject();
			ItemBaseMap.Add(tempItem->TagName, tempItem->GetClass());
		}
	}

	for (TActorIterator<ATriggerBox> Iter(GetWorld()); Iter; ++Iter)
	{
		TriggerBoxes.Add(*Iter);
		for (auto Box : TriggerBoxes)
		{
			Box->OnActorBeginOverlap.AddDynamic(this, &ACGameModeBase::OnTriggerBoxOverlap);
		}
	}

	for (TActorIterator<AEnemyCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		if (*Iter)
		{
			BossCharacter = *Iter;
		}
	}

	for (TActorIterator<Auwol_test> Iter(GetWorld()); Iter; ++Iter)
	{
		if (*Iter)
		{
			PlayerCharacter = *Iter;
		}
	}
}

void ACGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		PC->StartFade(true);
	}
}

UClass* ACGameModeBase::GetItemClassByName(FName ItemName)
{
	FString Msg;
	if (ensure(!ItemName.IsNone()))
	{
		if (ensure(ItemBaseMap.Contains(ItemName))) {
			return ItemBaseMap[ItemName]/*->GetClass()*/;
		}
		else
		{
			Msg = FString::Printf(TEXT("ItemMap in CGameModeBase does not contain Key, %s"), *ItemName.ToString());
		}
	}
	else
	{
		Msg = FString::Printf(TEXT("Name was not passed in! ItemName is Empty!"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Msg, true);
	return nullptr;
}

TArray<TSubclassOf<UCItemBase>> ACGameModeBase::GetItems()
{
	return ItemBaseClasses;
}

TArray<FStruct_Progression> ACGameModeBase::GetProgressions(EPlayerStat StatType) const
{
	TArray<FStruct_Progression*> ProgRows;
	switch (StatType)
	{
	case EPlayerStat::HEALTH:
	{
		DT_HealthProgression->GetAllRows<FStruct_Progression>("String", ProgRows);
		break;
	}
	case EPlayerStat::MANA:
	{
		DT_ManaProgression->GetAllRows<FStruct_Progression>("String", ProgRows);
		break;
	}
	case EPlayerStat::STAMINA:
	{
		DT_StaminaProgression->GetAllRows<FStruct_Progression>("String", ProgRows);
		break;
	}
	}


	TArray<FStruct_Progression> ProgressionArr;
	for (int i = 0; i < ProgRows.Num(); ++i)
	{
		FStruct_Progression ProgressionRow = { ProgRows[i]->Level, ProgRows[i]->Amount,ProgRows[i]->LevelupCost };

		ProgressionArr.Add(ProgressionRow);
	}

	return ProgressionArr;
}

FStruct_Progression ACGameModeBase::GetCurrentProgressionOf(EPlayerStat StatType, int Level)
{
	TArray<FStruct_Progression> temp = GetProgressions(StatType);
	ensureAlways(Level <= temp.Num() && Level > 0);
	return temp[Level - 1];
}

TArray<FStruct_Level> ACGameModeBase::GetCurrentLevels() const
{
	FString Context = "StringContext";
	TArray< FStruct_Level*> StatsPtr;
	TArray< FStruct_Level> Stats;
	DT_CurrentLevels->GetAllRows<FStruct_Level>(Context, StatsPtr);
	for (FStruct_Level* stat : StatsPtr)
	{
		FStruct_Level Level = *stat;
		Stats.Add(Level);
	}
	return Stats;
}

FName ACGameModeBase::GetStatName(EPlayerStat PlayerStatEnum)
{
	switch (PlayerStatEnum)
	{
	case EPlayerStat::HEALTH:
	{
		return FName("Health");
	}
	case EPlayerStat::MANA:
	{
		return FName("Mana");
	}
	case EPlayerStat::STAMINA:
	{
		return FName("Stamina");
	}
	default:
	{
		return FName();
	}
	}
}

FName ACGameModeBase::GetStatName(EPlayerStat PlayerStatEnum) const
{
	switch (PlayerStatEnum)
	{
	case EPlayerStat::HEALTH:
	{
		return FName("Health");
	}
	case EPlayerStat::MANA:
	{
		return FName("Mana");
	}
	case EPlayerStat::STAMINA:
	{
		return FName("Stamina");
	}
	default:
	{
		return FName();
	}
	}
}

void ACGameModeBase::IntroFade()
{

}

void ACGameModeBase::OnTriggerBoxOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping with %s"), *GetNameSafe(OtherActor));

	auto Projectile = Cast<ACMagicProjectile>(OtherActor);
	if (Projectile && OverlappedActor->ActorHasTag("ProjectileBlockTag"))
	{
		Projectile->Destroy();
		return;
	}

	auto Player = Cast<Auwol_test>(OtherActor);
	if (Player)
	{
		auto Game = Player->GetComponentByClass<UCGameplayComponent>();
		if (Game)
		{
			Game->StopAllActions(Player);
		}

		auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
		if (PC)
		{
			PlayerCharacter->GetCharacterMovement()->DisableMovement();
		}
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACGameModeBase::PlayerPauseOverlapsed, 3.f);


	}
}

void ACGameModeBase::PlayerPauseOverlapsed()
{
	//Fade out Screen to black
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		if (PC->OnFadeSuccess.IsBound())
		{
			PC->OnFadeSuccess.RemoveAll(this);
		}

		FScriptDelegate Dele;
		Dele.BindUFunction(this, "OnFadeSuccess_BattleStart");
		PC->OnFadeSuccess.Add(Dele);
		PC->StartFade(false);
	}
}

void ACGameModeBase::OnFadeSuccess_BattleStart(AActor* Actor)
{
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC->OnFadeSuccess.IsBound())
	{
		PC->OnFadeSuccess.RemoveAll(this);
	}

	SetupBindings();
	if (bIsFirstTimeLoading)
	{
		PlayCinematic();
	}
	else
		StartBattle();
}

void ACGameModeBase::SetupBindings()
{
	auto PlayerComp = PlayerCharacter->GetComponentByClass<UCPlayerAttributeComp>();
	if (PlayerComp)
	{
		if (PlayerComp->OnDead.IsBound())
		{
			PlayerComp->OnDead.Clear();
		}
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "OnPlayerDead");
		PlayerComp->OnDead.Add(Delegate);
	}

	auto AttributeComp = BossCharacter->GetComponentByClass<UCAttributeComponent>();
	if (AttributeComp)
	{
		if (AttributeComp->OnDead.IsBound())
		{
			AttributeComp->OnDead.Clear();
		}
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "OnBossDead");
		AttributeComp->OnDead.Add(Delegate);

	}
}


void ACGameModeBase::PlayCinematic()
{
	//Play Cinematic
	ALevelSequenceActor* ActorSequence;
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossIntroSequence,
		FMovieSceneSequencePlaybackSettings(), ActorSequence);
	if (LevelSequencePlayer)
	{
		if (LevelSequencePlayer->OnFinished.IsBound())
		{
			LevelSequencePlayer->OnFinished.Clear();
		}

		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "OnSequenceFinished");
		LevelSequencePlayer->OnFinished.Add(Delegate);
		LevelSequencePlayer->Play();
	}
	else
	{
		StartBattle();
	}
}

void ACGameModeBase::OnSequenceFinished()
{
	bIsFirstTimeLoading = false;
	StartBattle();
}

void ACGameModeBase::StartBattle()
{
	//Relocate player to his/her respective positions
	PlayerCharacter->SetActorLocation(PlayerLocation_Battle);

	//enable boss battle state 
	auto AIController = Cast<ACAIController>(BossCharacter->GetController());
	if (AIController)
	{
		AIController->SetBattleState();
	}
	//Spawn Blocker that keeps player from leaving
	if (ensure(BlockingWallClass))
	{
		FTransform SpawnTransform;
		SpawnTransform.SetTranslation(BlockingWallLocation);
		SpawnTransform.SetRotation(TriggerBoxes[0]->GetActorRotation().Quaternion());
		BlockingWall = GetWorld()->SpawnActor<AActor>(BlockingWallClass, SpawnTransform);
	}
	//fadeout to scene
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		PC->StartFade(true);
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	//Disable Trigger boxes
	for (auto Box : TriggerBoxes)
	{
		Box->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ACGameModeBase::OnPlayerDead()
{
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		PlayerCharacter->DisableInput(PC);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACGameModeBase::PausePeriod, 2.5f);
	}
}

void ACGameModeBase::PausePeriod()
{
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		Widget_GameOver = CreateWidget<UUserWidget_Gameplay>(PC, Widget_GameOverClass);
		if (ensure(Widget_GameOver))
		{
			Widget_GameOver->AddToViewport(-1);
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, "ButtonClick_BattleReset");
			Widget_GameOver->OnButtonClicked.Add(Delegate);
			PC->bShowMouseCursor = true;
		}
	}
}


void ACGameModeBase::ButtonClick_BattleReset()
{
	//Fade out Screen to black
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		if (PC->OnFadeSuccess.IsBound())
		{
			PC->OnFadeSuccess.RemoveAll(this);
		}

		FScriptDelegate Dele;
		Dele.BindUFunction(this, "OnFadeSuccess_BattleReset");
		PC->OnFadeSuccess.Add(Dele);
		PC->StartFade(false);
	}
}


void ACGameModeBase::OnFadeSuccess_BattleReset(AActor* Actor)
{
	ResetBattle();
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		if (PC->OnFadeSuccess.IsBound())
		{
			PC->OnFadeSuccess.RemoveAll(this);
		}
		PC->StartFade(true);
	}
}

void ACGameModeBase::ResetBattle()
{
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		//Reset player
		PlayerCharacter->EnableInput(PC);
		auto PlayerComp = PlayerCharacter->GetComponentByClass<UCAttributeComponent>();
		if (PlayerComp)
		{
			PlayerComp->RecoverFullHealth();
		}

		for (TActorIterator<APlayerStart> Iter(GetWorld()); Iter; ++Iter)
		{
			PlayerCharacter->SetActorLocation(Iter->GetActorLocation());
		}

		//Reset Widget Gameover
		Widget_GameOver->RemoveFromParent();
		PC->bShowMouseCursor = false;

		//Reset Boss
		auto AttriComp = BossCharacter->GetComponentByClass<UCAttributeComponent>();
		if (AttriComp)
		{
			AttriComp->RecoverFullHealth();
		}
		BossCharacter->SetActorLocation(BossLocation_Battle);
		auto AIController = Cast<ACAIController>(BossCharacter->GetController());
		if (AIController)
		{
			AIController->SetStartState();
		}

		//Reset blocking wall
		if (BlockingWall)
		{
			BlockingWall->Destroy();
		}

		//Re-enable Trigger boxes
		for (auto Box : TriggerBoxes)
		{
			Box->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}

}

void ACGameModeBase::OnBossDead()
{
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		PlayerCharacter->DisableInput(PC);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACGameModeBase::PausePeriodWon, 2.5f);
	}
}

void ACGameModeBase::PausePeriodWon()
{
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		Widget_Victory = CreateWidget<UUserWidget_Gameplay>(PC, Widget_VictoryClass);
		if (ensure(Widget_Victory))
		{
			Widget_Victory->AddToViewport(-1);
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, "ButtonClick_BattleOver");
			Widget_Victory->OnButtonClicked.Add(Delegate);
			PC->bShowMouseCursor = true;
		}
	}
}

void ACGameModeBase::ButtonClick_BattleOver()
{
	//Fade out Screen to black
	auto PC = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		if (PC->OnFadeSuccess.IsBound())
		{
			PC->OnFadeSuccess.RemoveAll(this);
		}

		FScriptDelegate Dele;
		Dele.BindUFunction(this, "OnFadeSuccess_BattleOver");
		PC->OnFadeSuccess.Add(Dele);
		PC->StartFade(false);
	}
}

void ACGameModeBase::OnFadeSuccess_BattleOver(AActor* Actor)
{
	UGameplayStatics::OpenLevel(this, "IntroLev");
}

void ACGameModeBase::RestoreTime(AActor* ActorContext)
{
	UGameplayStatics::SetGlobalTimeDilation(ActorContext, 1.f);

	UCGameplayLibrary::AddCurrency(ActorContext);
}

/*void ACGameModeBase::RespawnPlayer(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}*/

void ACGameModeBase::SlowDownTime(AActor* ActorContext)
{
	UGameplayStatics::SetGlobalTimeDilation(ActorContext, .1f);
	FTimerDelegate Delegate;
	FTimerHandle Handle;
	Delegate.BindUFunction(this, "RestoreTime", ActorContext);
	GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, .05f, false);
	UGameplayStatics::PlaySound2D(this, DodgeSound);
}
