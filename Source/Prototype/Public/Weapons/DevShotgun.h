// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "DevShotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ADevShotgun : public AWeaponBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ADevShotgun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ShootPrimary() override;

private:
	UPROPERTY(EditAnywhere, Category = "Bullets")
		int m_ShotsFired = 4;

	UPROPERTY(EditAnywhere, Category = "Bullets")
		float m_SpreadAngle = 15.f;

	UPROPERTY(EditAnywhere, Category = "Bullets")
		float m_MaxZOffset = 1.f;
};
