// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UCapsuleComponent;
class ABullet;

UCLASS()
class PROTOTYPE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
		UCapsuleComponent* m_pCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		UArrowComponent* m_pShootLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		float m_MaxLineTraceDistance = 100000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		bool m_DrawDebugLines = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		bool m_PrintDebugHelp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* m_pMesh;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		bool m_CanHoldDownButton = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		float m_Damage = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		float m_FireRate = 5.f;
	UPROPERTY(VisibleAnywhere, Category = "Shooting")
		float m_FireRateTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		USoundBase* m_ShootSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		TSubclassOf<ABullet> m_BulletType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		float m_BulletMaxLifeSpawn = 20.f;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		float m_RotationSpeed = 10.f;


	virtual void BeginPlay() override;

	bool IsPickUp();

	void ResetFireRateTime();
	void ShootBullet();
	void ShootBullet(const FVector& direction);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Pure virtual functions
	// To be implemented by the inherited classes
	virtual void ShootPrimary() PURE_VIRTUAL(AWeaponBase::ShootPrimary, );
	//virtual void ShootPrimary();

	bool CanShoot();
	bool IsFiring();
	void IsFiring(bool firing);

	void ResetLocals();
	void SetVisibility(bool visible);

private:
	bool m_IsFiring = false;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
};
