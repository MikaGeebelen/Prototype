// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UStaticMeshComponent;
class UHealthComponent;
class UWeaponManagerComponent;
class UArrowComponent;

UCLASS()
class PROTOTYPE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* m_pBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* m_pLookDir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UHealthComponent* m_pHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "weapon");
	UWeaponManagerComponent* m_pWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	UArrowComponent* m_pGunPosition;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings");
	float m_ActivateDelay = 5.f;
	float m_Time = 0.f;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};