// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "DevGun.generated.h"

class ABullet;

UCLASS()
class PROTOTYPE_API ADevGun : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADevGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		TSubclassOf<ABullet> m_BulletType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ShootPrimary() override;
};
