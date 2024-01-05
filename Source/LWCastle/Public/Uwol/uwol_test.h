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
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* tpsCamComp;

	// Horizontal rotation
	void Turn(float value);
	// Vertical rotation
	void LookUp(float value);

	// moving velocity
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 1000;
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

};
