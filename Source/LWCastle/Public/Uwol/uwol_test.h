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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCPlayerAttributeComp> PlayerAttributeComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCGameplayComponent> GameplayComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCCombatComponent> CombatComp;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Dodge();

	// Horizontal rotation
	void Turn(float value);
	// Vertical rotation
	void LookUp(float value);

	// Horizontal movement
	void InputHorizontal(float value);
	// Vertical movement
	void InputVertical(float value);

	// Jump
	void InputJump();

	// Player movement
	void Move(float DeltaTime);

	// fire
	void InputFirePressed();
	void InputFireReleased();

	void SniperAimHold();
	void SniperAimRelease();
	
	void SwitchElement();

	void Attack_Melee();

	void RunP();

	UFUNCTION()
	void OnStaminaDepleted();
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TSubclassOf<UCAction> ActionEffectStunClass;
	bool bIsStunned = false;

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


	// moving velocity
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 200;
	// direction
	FVector direction;


	// DefaultMagic skeletal mesh
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	// DafulatMagic factory
	UPROPERTY(EditDefaultsOnly, Category = MagicFactory)
	TSubclassOf<class ADefaultMagic> defaultmagicfac;

	// CrossHair
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// CrossHair Instance
	UPROPERTY()
	class UUserWidget* _crosshairUI;

	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly, Category = FocusUI)
	TSubclassOf<class UUserWidget> sniperFac;
	UPROPERTY()
	class UUserWidget* _FocusUI;


	float movespeed = 1.0;

	//void speedchange();

	//void RunR();

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
