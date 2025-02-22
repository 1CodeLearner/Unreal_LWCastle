// Fill out your copyright notice in the Description page of Project Settings.

#include "Uwol/uwol_test.h"
#include "Uwol/DefaultMagic.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Justin/AComponents/CCombatComponent.h"
#include "Justin/AComponents/CGameplayComponent.h"
#include "Uwol/PlayerAnim.h"
#include "Justin/AComponents/CPlayerAttributeComp.h"

// Sets default values
Auwol_test::Auwol_test()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Load and set location, rotation
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/uwol_blueprints/Animation_Retarget/Elaina/Real_Character/Crouch_Walk_Forward.Crouch_Walk_Forward'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// Set Camera
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SocketOffset = FVector(0, 50, 60);
	springArmComp->TargetArmLength = 200;
	//springArmComp->bUsePawnControlRotation = true;
	//springArmComp->bUsePawnControlRotation = false;

	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	//tpsCamComp->bUsePawnControlRotation = false;

	//bUseControllerRotationYaw = true;
	//bUseControllerRotationYaw = false;

	// Default Magic 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/untitled_category/untitled_asset/Nobeta_Staff_mesh.Nobeta_Staff_mesh'"));
	// 데이터로드 성공
	if (TempGunMesh.Succeeded())
	{
		// 스켈레탈 메시 데이터 할당
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		FName SocketName = TEXT("WeaponSocket");
		gunMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		// 위치 조정
		//gunMeshComp->SetRelativeLocation(FVector(-38, -2, 30));
		//gunMeshComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		//gunMeshComp->SetRelativeScale3D(FVector(1.0f));

	}

	PlayerAttributeComp = CreateDefaultSubobject<UCPlayerAttributeComp>("PlayerAttributeComp");
	PlayerAttributeComp->OnStaminaDepleted.AddDynamic(this, &Auwol_test::OnStaminaDepleted);
	GameplayComp = CreateDefaultSubobject<UCGameplayComponent>("GameplayComp");
	CombatComp = CreateDefaultSubobject<UCCombatComponent>("CombatComp");
}


// Called when the game starts or when spawned
void Auwol_test::BeginPlay()
{
	Super::BeginPlay();

	// Crosshair
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();

	_FocusUI = CreateWidget(GetWorld(), sniperFac);

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	CombatComp->InitializeWidget();
}

void Auwol_test::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CombatComp->Initialize();
}

// Called every frame
void Auwol_test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameplayComp->ActiveGameplayTags.HasAny(StunContainer))
	{
		return;
	}
	Move(DeltaTime);

	static FGameplayTag SprintTag = FGameplayTag::RequestGameplayTag("Movement.Sprint");
	static FGameplayTag ADSTag = FGameplayTag::RequestGameplayTag("Combat.ADS");
	static FGameplayTag MagicTag = FGameplayTag::RequestGameplayTag("Combat.Magic");

	static FGameplayTagContainer Container;
	Container.AddTag(SprintTag);
	Container.AddTag(ADSTag);

	if (GameplayComp->ActiveGameplayTags.HasAllExact(Container))
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}
	else if(GameplayComp->ActiveGameplayTags.HasTag(ADSTag))
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
	}	
	else if(GameplayComp->ActiveGameplayTags.HasTag(SprintTag))
	{
		if(GameplayComp->ActiveGameplayTags.HasTag(MagicTag))
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
		else
			GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

// Called to bind functionality to input
void Auwol_test::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &Auwol_test::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &Auwol_test::LookUp);

	// Horizontal binding
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &Auwol_test::InputHorizontal);
	// Vertical binding
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &Auwol_test::InputVertical);

	// Jump binding
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &Auwol_test::InputJump);

	// Fire binding
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &Auwol_test::InputFirePressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &Auwol_test::InputFireReleased);



	// Aim binding
	PlayerInputComponent->BindAction(TEXT("Focus"), IE_Pressed, this, &Auwol_test::SniperAimHold);
	// Release binding
	PlayerInputComponent->BindAction(TEXT("Focus"), IE_Released, this, &Auwol_test::SniperAimRelease);

	// Rkey
	PlayerInputComponent->BindAction(TEXT("SwitchElement"), IE_Pressed, this, &Auwol_test::SwitchElement);

	// Dodge Binding
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &Auwol_test::Dodge);

	// Run Binding
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &Auwol_test::RunP);
	//PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &Auwol_test::RunR);

	// Attack Binding
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &Auwol_test::Attack_Melee);


	PlayerInputComponent->BindAction(TEXT("StartCharging"), IE_Released, this, &Auwol_test::StartCharging);
}

void Auwol_test::OnStaminaDepleted()
{
	GameplayComp->AddAction(this, ActionEffectStunClass);
}

FVector Auwol_test::GetPawnViewLocation() const
{
	return tpsCamComp->GetComponentLocation();
}

void Auwol_test::Turn(float value)
{
	AddControllerYawInput(value);
}

void Auwol_test::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void Auwol_test::InputHorizontal(float value)
{
	direction.Y = value;
}

void Auwol_test::InputVertical(float value)
{
	direction.X = value;
}

void Auwol_test::InputJump()
{
	GameplayComp->StartActionByName(this, "Jump");
}

void Auwol_test::Move(float DeltaTime)
{
	FRotator rot = GetControlRotation();
	FVector forward = UKismetMathLibrary::GetForwardVector(rot);
	FVector right = UKismetMathLibrary::GetRightVector(rot);
	direction = direction.Y * right + direction.X * forward;
	direction.Normalize();

	static FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag("Movement.Jump");

	if (!GameplayComp->ActiveGameplayTags.HasAny(MeleeGrantedTags) || GameplayComp->ActiveGameplayTags.HasTagExact(JumpTag))
	{
		AddMovementInput(direction, movespeed);
	}
	direction = FVector::ZeroVector;
}

void Auwol_test::InputFirePressed()
{
	/*auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();*/

	GameplayComp->StartActionByName(this, "Attack");

	//isattackingmagic = true;

	// 발사
	//FTransform fireposition = gunMeshComp->GetSocketTransform(TEXT("FirePosition2"));

	// Linetrace
	//FVector startPos = tpsCamComp->GetComponentLocation();
	//FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
	//FHitResult hitInfo;
	//FCollisionQueryParams params;
	//params.AddIgnoredActor(this);
	//bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	//if (bHit)
	//{
	//	FTransform magicTrans;
	//	magicTrans.SetLocation(hitInfo.ImpactPoint);
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MagicEffectFactory, magicTrans);
	//}
	//FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, &Auwol_test::speedchange, 7.0f, false);
	//isattackingmagic = false;

	// 발사
	// GetWorld()->SpawnActor<ADefaultMagic>(defaultmagicfac, fireposition);


}

void Auwol_test::InputFireReleased()
{
	GameplayComp->CompleteActionByName(this, "Attack");
}

void Auwol_test::SniperAimHold()
{
	GameplayComp->StartActionByName(this, "ADS");

}

void Auwol_test::SniperAimRelease()
{
	GameplayComp->CompleteActionByName(this, "ADS");
}

void Auwol_test::SwitchElement()
{
	static long value = 0;
	++value;
	UE_LOG(LogTemp, Warning, TEXT("Value: %d"), value);
	if (value % 2 == 1)
	{
		CombatComp->SwitchElementByName("Arcane");
	}
	else
	{
		CombatComp->SwitchElementByName("Ice");
	}

	/*GetCharacterMovement()->MaxWalkSpeed /= 3.0;
	//movespeed = 0.5f;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &Auwol_test::speedchange, 7.0f, false);*/
}

/*

void Auwol_test::speedchange()
{
	//movespeed = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed *= 3.0;
}*/

void Auwol_test::Dodge()
{
	if (!GameplayComp->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Movement.Roll")) && !bIsStunned)
	{
		if (PlayerAttributeComp->GetCurrentStamina() <= 0.f)
		{
			bIsStunned = true;
			OnStaminaDepleted();
			return;
		}
		GameplayComp->StartActionByName(this, "Roll");
	}
}

void Auwol_test::RunP()
{
	//movespeed = 1.5f;
	//GetCharacterMovement()->MaxWalkSpeed *= 3.0;
	GameplayComp->StartActionByName(this, "Sprint");
}

/*
void Auwol_test::RunR()
{
	//movespeed = 1.0f;
	//GetCharacterMovement()->MaxWalkSpeed /= 3.0;
	GameplayComp->CompleteActionByName(this, "Sprint");
}
*/

void Auwol_test::Attack_Melee()
{
	if (isDuringAttack)
		return;

	if (GameplayComp->ActiveGameplayTags.HasAny(MeleeBlockTags))
		return;

	GameplayComp->StartActionByName(this, "Melee");
	FOnMontageEnded BlendDelegate;
	FOnMontageEnded EndDelegate;
	BlendDelegate.BindUFunction(this, "OnBlendingOutStarted");
	EndDelegate.BindUObject(this, &Auwol_test::OnEnded);
	UPlayerAnim* Anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	if (Anim->OnMontageBlendingOut.IsBound())
	{
		Anim->OnMontageBlendingOut.Clear();
	}

	if (Anim->OnMontageEnded.IsBound())
	{
		Anim->OnMontageEnded.Clear();
	}

	switch (ComboAttack_Num)
	{
	case 0:
		PlayAnimMontage(Attack_MeleeAnim1, 1.0f);
		Anim->Montage_SetBlendingOutDelegate(BlendDelegate, Attack_MeleeAnim1);
		Anim->Montage_SetEndDelegate(EndDelegate, Attack_MeleeAnim1);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	case 1:
		PlayAnimMontage(Attack_MeleeAnim2, 1.0f);
		Anim->Montage_SetBlendingOutDelegate(BlendDelegate, Attack_MeleeAnim2);
		Anim->Montage_SetEndDelegate(EndDelegate, Attack_MeleeAnim2);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	case 2:
		PlayAnimMontage(Attack_MeleeAnim3, 1.0f);
		Anim->Montage_SetBlendingOutDelegate(BlendDelegate, Attack_MeleeAnim3);
		Anim->Montage_SetEndDelegate(EndDelegate, Attack_MeleeAnim3);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	case 3:
		PlayAnimMontage(Attack_MeleeAnim4, 1.0f);
		Anim->Montage_SetBlendingOutDelegate(BlendDelegate, Attack_MeleeAnim4);
		Anim->Montage_SetEndDelegate(EndDelegate, Attack_MeleeAnim4);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	default:
		ComboAttack_Num = 0;
	}
}

void Auwol_test::Attack_Melee_End()
{
	isDuringAttack = false;
}

void Auwol_test::MeleeCombo_Reset()
{
	ComboAttack_Num = 0;

}

void Auwol_test::OnBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted)
{
	UPlayerAnim* Anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	UE_LOG(LogTemp, Warning, TEXT("ActiveMontage: %s"), *GameplayComp->ActiveGameplayTags.ToString());//*GetNameSafe(Anim->GetCurrentActiveMontage()));
	if (GameplayComp->ActiveGameplayTags.Num() == 1 && GameplayComp->ActiveGameplayTags.HasTagExact(FGameplayTag::RequestGameplayTag("Combat.Melee")))
	{
		GameplayComp->CompleteActionByName(this, "Melee");
	}

	if (bInterrupted && isDuringAttack)
	{
		Attack_Melee_End();
		MeleeCombo_Reset();
		GameplayComp->CompleteActionByName(this, "Melee");
	}
	else if (bInterrupted && !isDuringAttack && GameplayComp->ActiveGameplayTags.HasAny(MeleeBlockTags))
	{
		GameplayComp->CompleteActionByName(this, "Melee");
	}
}

void Auwol_test::OnEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		Attack_Melee_End();
		MeleeCombo_Reset();
		UE_LOG(LogTemp, Warning, TEXT("Here %s"), *GetNameSafe(Montage));
		//GameplayComp->ActiveGameplayTags.RemoveTags(MeleeGrantedTags);
		GameplayComp->CompleteActionByName(this, "Melee");

		UPlayerAnim* Anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	}
}

void Auwol_test::StartCharging()
{
	GameplayComp->StartActionByName(this, "Charging");
}

void Auwol_test::StartTeleport()
{
	GetController()->SetIgnoreMoveInput(true);
	GetMesh()->SetVisibility(false);
	gunMeshComp->SetVisibility(false);
	FVector Vel = GetCharacterMovement()->Velocity;
	FVector Dir;

	if (FVector::DotProduct(Vel, Vel) == 0)
	{
		Dir = GetActorForwardVector() * -1;
	}
	else
	{
		Vel.Normalize();
		Dir = UKismetMathLibrary::GetForwardVector(FRotationMatrix::MakeFromX(Vel).Rotator());//UKismetMathLibrary::GetForwardVector();
	}

	Dir *= TeleportDistance;
	FVector Dest = GetActorLocation() + Dir;

	TeleportTo(Dest, GetActorRotation());
}

void Auwol_test::EndTeleport()
{
	GetController()->ResetIgnoreMoveInput();
	gunMeshComp->SetVisibility(true);
	GetMesh()->SetVisibility(true);
}
