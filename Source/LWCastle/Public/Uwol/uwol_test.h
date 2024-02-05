// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "uwol_test.generated.h"


class UCAction;
class UCActionEffect;
class UCGameplayComponent;
class UCCombatComponent;
class UCPlayerAttributeComp;
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

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test")
	TObjectPtr<UCPlayerAttributeComp> PlayerAttributeComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test")
	TObjectPtr<UCGameplayComponent> GameplayComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test")
	TObjectPtr<UCCombatComponent> CombatComp;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UFUNCTION(BlueprintCallable)
	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StunContainer;

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
	UPROPERTY(EditDefaultsOnly, Category = MagicFactory)
	TSubclassOf<class ADefaultMagic> defaultmagicfac;

	// fire
	void InputFirePressed();
	void InputFireReleased();

	// CrossHair
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// CrossHair Instance
	UPROPERTY()
	class UUserWidget* _crosshairUI;

	void SniperAimHold();
	void SniperAimRelease();
	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly, Category = FocusUI)
	TSubclassOf<class UUserWidget> sniperFac;
	UPROPERTY()
	class UUserWidget* _FocusUI;

	void SwitchElement();

	float movespeed = 1.0;

	//void speedchange();

	void RunP();

	//void RunR();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* pDodgeMontage;

	void Dodge();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsDodging = false;

	void ResetDodgeState();

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* DodgeAnimMontage;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartDodgeAnimation();

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* MagicEffectFactory;

	void Attack_Melee();
	UFUNCTION(BlueprintCallable)
	void Attack_Melee_End();
	UFUNCTION(BlueprintCallable)
	void MeleeCombo_Reset();


	UPROPERTY(EditDefaultsOnly, Category = Melee)
	UAnimMontage* Attack_MeleeAnim1;

	UPROPERTY(EditDefaultsOnly, Category = Melee)
	UAnimMontage* Attack_MeleeAnim2;


	UPROPERTY(EditDefaultsOnly, Category = Melee)
	UAnimMontage* Attack_MeleeAnim3;


	UPROPERTY(EditDefaultsOnly, Category = Melee)
	UAnimMontage* Attack_MeleeAnim4;

	UFUNCTION()
	void OnBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FGameplayTagContainer MeleeBlockTags;
	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FGameplayTagContainer MeleeGrantedTags;

	UFUNCTION()
	void StartCharging();

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDistance;
	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void StartTeleport();
	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void EndTeleport();

	bool isDuringAttack;
	// 좌클릭 공격 사용 중
	bool isattackingmagic = false;
	int ComboAttack_Num;

};
