// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UArrowComponent;
class UWeaponManagerComponent;
class USpringArmComponent;
class UHealthComponent;
class UCameraComponent;
class AWeaponBase;

UCLASS()
class PROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
		UHealthComponent* m_pHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		UArrowComponent* m_pGunPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		UArrowComponent* m_pGunDropPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		UWeaponManagerComponent* m_pWeaponManager;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
	//	AWeaponBase* m_pWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* m_pCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* m_pSpringArm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UWeaponManagerComponent* GetWeaponManager();
	void PickUpWeapon(AWeaponBase* weapon);

private:

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void RestartLevel();

	//Movement
	void VerticalMovement(float axis);
	void HorizontalMovement(float axis);
	void Jump();

	//Looking
	UPROPERTY(EditAnywhere, Category = "Looking")
		float m_MaxVerticalAngle = 45.f;
	UPROPERTY(EditAnywhere, Category = "Looking")
		float m_MinVerticalAngle = 45.f;

	FRotator m_OriginalRotation;

	void VerticalLook(float axis);
	void HorizontalLook(float axis);
	void LookInCameraDirection();

	//Shooting
	bool m_UpdateWeaponPos = true;
	bool m_IsRotationOk = false;
	bool m_FireWeapon = false;
	FRotator m_DefaultCameraRotation;

	void ShootWeapon();
	void FireWeapon();
	void ReleaseWeapon();
	void UpdateWeaponRotation();
	void SelectFirstWeapon();
	void SelectSecondWeapon();
	void TestFunction();
};
