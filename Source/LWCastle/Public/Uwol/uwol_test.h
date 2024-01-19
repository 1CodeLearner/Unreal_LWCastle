// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "uwol_test.generated.h"

UCLASS()
class LWCASTLE_API Auwol_test : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Auwol_test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category=Camera)
	class UCameraComponent* tpsCamComp;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	//UAnimMontage* m_pDodgeMomtage;

	//void Dodge();
	//bool m_bIsDodging = false;


	// Horizontal rotation
	void Turn(float value);
	// Vertical rotation
	void LookUp(float value);

	// moving velocity
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 200;
	// direction
	FVector direction;

	// Horizontal movement
	void InputHorizontal(float value);
	// Vertical movement
	void InputVertical(float value);

	// Jump
	void InputJump();

	// Player movement
	void Move(float DeltaTime);

	// DefaultMagic skeletal mesh
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	// DafulatMagic factory
	UPROPERTY(EditDefaultsOnly, Category=MagicFactory)
	TSubclassOf<class ADefaultMagic> defaultmagicfac;

	// fire
	void InputFire();

	// CrossHair
	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// CrossHair Instance
	class UUserWidget* _crosshairUI;

	void SniperAim();
	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly, Category=FocusUI)
	TSubclassOf<class UUserWidget> sniperFac;
	class UUserWidget* _FocusUI;

	void CastR();

	float movespeed = 1.0;

	void speedchange();
	void RunP();
	void RunR();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* pDodgeMontage;

	void Dodge();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool IsDodging = false;

	void ResetDodgeState();

	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* DodgeAnimMontage;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartDodgeAnimation();

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* MagicEffectFactory;

	void Attack_Melee();
	void Attack_Melee_End();


	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Attack_MeleeAnim1;
	
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Attack_MeleeAnim2;

	
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Attack_MeleeAnim3;

	
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Attack_MeleeAnim4;


	bool isDuringAttack;
	// 좌클릭 공격 사용 중
	bool isattackingmagic = false;
	int ComboAttack_Num;

};
