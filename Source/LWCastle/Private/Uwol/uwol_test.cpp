// Fill out your copyright notice in the Description page of Project Settings.


#include "Uwol/uwol_test.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Uwol/DefaultMagic.h"
#include <Kismet/GameplayStatics.h>

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
	springArmComp->SetRelativeLocation(FVector(0, 0, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	//springArmComp->bUsePawnControlRotation = false;
	
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	//bUseControllerRotationYaw = false;

	// Default Magic ���̷�Ż�޽� ������Ʈ ���
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	gunMeshComp->SetupAttachment(GetMesh());
	// ���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/untitled_category/untitled_asset/Nobeta_Staff_mesh.Nobeta_Staff_mesh'"));
	// �����ͷε� ����
	if (TempGunMesh.Succeeded())
	{
		// ���̷�Ż �޽� ������ �Ҵ�
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// ��ġ ����
		gunMeshComp->SetRelativeLocation(FVector(-38, -2, 30));
		// ���� ����
		gunMeshComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
}


// Called when the game starts or when spawned
void Auwol_test::BeginPlay()
{
	Super::BeginPlay();
	
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

	// �̵������� �����ǥ��� ��ȯ
	direction = FTransform(GetControlRotation()).TransformVector(direction);

	// Player movement
	
	FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	
	if (!direction.IsNearlyZero())
	{
		// ī�޶� ���� �������� ĳ���� ȸ��
		FRotator camRot = vt.ToOrientationRotator();
		SetActorRotation(camRot);
	}
	//AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void Auwol_test::InputFire()
{
	// ���� ����
	FTransform fireposition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	UCameraComponent* CameraComponent = FindComponentByClass<UCameraComponent>();
	FVector CameraForwardVector;
	if (CameraComponent)
	{
		CameraForwardVector = CameraComponent->GetForwardVector();
		// CameraForwardVector�� ����Ͽ� ���ϴ� �۾� ����
	}
	UGameplayStatics::GetPlayerController(this, 0);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, fireposition.GetLocation(), CameraForwardVector, ECC_GameTraceChannel1);


	GetWorld()->SpawnActor<ADefaultMagic>(defaultmagicfac, fireposition);

}