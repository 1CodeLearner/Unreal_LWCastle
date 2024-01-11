// Fill out your copyright notice in the Description page of Project Settings.

#include "Uwol/uwol_test.h"
#include "Uwol/DefaultMagic.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
Auwol_test::Auwol_test()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Load and set location, rotation
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/playermodel/Nobeta_mesh.Nobeta_mesh'"));
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
	gunMeshComp->SetupAttachment(GetMesh());
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/untitled_category/untitled_asset/Nobeta_Staff_mesh.Nobeta_Staff_mesh'"));
	// 데이터로드 성공
	if (TempGunMesh.Succeeded())
	{
		// 스켈레탈 메시 데이터 할당
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 위치 조정
		gunMeshComp->SetRelativeLocation(FVector(-38, -2, 30));
		// 방향 조정
		gunMeshComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
}


// Called when the game starts or when spawned
void Auwol_test::BeginPlay()
{
	Super::BeginPlay();

	// Crosshair
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();

	_FocusUI = CreateWidget(GetWorld(), sniperFac);
	
}

// Called every frame
void Auwol_test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &Auwol_test::InputFire);

	// Aim binding
	PlayerInputComponent->BindAction(TEXT("Focus"), IE_Pressed, this, &Auwol_test::SniperAim);
	// Release binding
	PlayerInputComponent->BindAction(TEXT("Focus"), IE_Released, this, &Auwol_test::SniperAim);

	// Rkey
	PlayerInputComponent->BindAction(TEXT("R"), IE_Pressed, this, &Auwol_test::CastR);

	// Dodge Binding
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &Auwol_test::Dodge);
	
	// Run Binding
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &Auwol_test::RunP);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &Auwol_test::RunR);

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
	Jump();
}

void Auwol_test::Move(float DeltaTime)
{

	FRotator rot = GetControlRotation();
	FVector forward = UKismetMathLibrary::GetForwardVector(rot);
	FVector right = UKismetMathLibrary::GetRightVector(rot);
	direction = direction.Y * right + direction.X * forward;
	direction.Normalize();

	AddMovementInput(direction, movespeed);
	direction = FVector::ZeroVector;
}

void Auwol_test::InputFire()
{
	// 현재 방향
	FTransform fireposition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//UCameraComponent* CameraComponent = FindComponentByClass<UCameraComponent>();
	//FVector CameraForwardVector;
	//if (CameraComponent)
	//{
	//	CameraForwardVector = CameraComponent->GetForwardVector();
		// CameraForwardVector를 사용하여 원하는 작업 수행
	//}
	//UGameplayStatics::GetPlayerController(this, 0);
	//FHitResult HitResult;
	//GetWorld()->LineTraceSingleByChannel(HitResult, fireposition.GetLocation(), CameraForwardVector, ECC_GameTraceChannel1);


	GetWorld()->SpawnActor<ADefaultMagic>(defaultmagicfac, fireposition);

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

void Auwol_test::CastR()
{
	movespeed = 0.5f;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &Auwol_test::speedchange, 7.0f, false);
}

void Auwol_test::speedchange()
{
	movespeed = 1.0f;
}

void Auwol_test::Dodge()
{
	LaunchCharacter(GetActorForwardVector() * 2500, true, true);
}

void Auwol_test::RunP()
{
	//movespeed = 1.5f;
	GetCharacterMovement()->MaxWalkSpeed *= 2.0;
	
}

void Auwol_test::RunR()
{
	//movespeed = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed /= 2.0;
}