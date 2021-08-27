// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class PROTOTYPE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
		UArrowComponent* m_pRoot;

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

	virtual void BeginPlay() override;

	bool CanShoot();

	void ResetFireRateTime();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Pure virtual functions
	// To be implemented by the inherited classes
	virtual void ShootPrimary() PURE_VIRTUAL(AWeaponBase::ShootPrimary, );
	//virtual void ShootPrimary();

	bool IsFiring();
	void IsFiring(bool firing);

private:
	bool m_IsFiring = false;
};
