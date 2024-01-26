// Fill out your copyright notice in the Description page of Project Settings.

#include "Uwol/uwol_test.h"
#include "Uwol/DefaultMagic.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

#include "AssetTypeCategories.h"
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
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
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
	PlayerInputComponent->BindAction(TEXT("Focus"), IE_Pressed, this, &Auwol_test::SniperAim);
	// Release binding
	PlayerInputComponent->BindAction(TEXT("Focus"), IE_Released, this, &Auwol_test::SniperAim);

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

	if (isDuringAttack == false)
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

void Auwol_test::SniperAim()
{
	if (bSniperAim == false)
	{
		bSniperAim = true;
		_FocusUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
	}
	else
	{
		bSniperAim = false;
		_FocusUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
	}
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
	GameplayComp->StartActionByName(this, "Roll");

	/*if (!IsDodging)
	{
		UAnimInstance* pAnimInst = GetMesh()->GetAnimInstance();
		if (pAnimInst != nullptr)
		{
			IsDodging = true;

			//StartDodgeAnimation();

			const FVector PlayerForward = GetActorForwardVector();
			//LaunchCharacter(PlayerForward * 2500, true, true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &Auwol_test::ResetDodgeState, 1.0f, false);
		}
	}*/
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

void Auwol_test::ResetDodgeState()
{
	IsDodging = false;
}

void Auwol_test::StartDodgeAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(pDodgeMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
	}
}

void Auwol_test::Attack_Melee()
{
	switch (ComboAttack_Num)
	{
	case 0:
		PlayAnimMontage(Attack_MeleeAnim1, 1.0f);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	case 1:
		PlayAnimMontage(Attack_MeleeAnim2, 1.0f);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	case 2:
		PlayAnimMontage(Attack_MeleeAnim3, 1.0f);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	case 3:
		PlayAnimMontage(Attack_MeleeAnim4, 1.0f);
		isDuringAttack = true;
		ComboAttack_Num++;
		break;
	default:
		ComboAttack_Num = 0;

	}
	FTimerHandle TH_Attack_End;
	GetWorldTimerManager().SetTimer(TH_Attack_End, this, &Auwol_test::Attack_Melee_End, 1.7f, false);
}

void Auwol_test::Attack_Melee_End()
{
	isDuringAttack = false;
}

void Auwol_test::StartCharging()
{
	GameplayComp->StartActionByName(this, "Charging");
}
