// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UArrowComponent;
class UWeaponManagerComponent;
class UWeaponManagerComponent;
class UCameraComponent;
class AWeaponBase;

UCLASS()
class PROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		UArrowComponent* m_pGunPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		UWeaponManagerComponent* m_pWeaponManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
		AWeaponBase* m_pWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* m_pCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Movement
	void VerticalMovement(float axis);
	void HorizontalMovement(float axis);
	void Jump();

	//Looking
	void VerticalLook(float axis);
	void HorizontalLook(float axis);

	//Shooting
	bool m_UpdateWeaponPos = true;
	void ShootWeapon();
};
