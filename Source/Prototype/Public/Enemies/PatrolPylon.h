// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPylon.generated.h"

class UArrowComponent;
class UWeaponManagerComponent;

UCLASS()
class PROTOTYPE_API APatrolPylon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPylon();

	virtual AActor* GetArriveLocation(FVector otherPylon);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent*  m_pPillar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<AActor*> m_Entrances;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual AActor* GetStartLocation(FVector otherPylon);
	
	virtual bool SendPatrol();

	virtual bool IsPatrolDead();

	virtual void Destroyed() override;

	void ToggleGates(bool canPass);

	void ShootPlayer();

	void SpawnDefenses();

	void AreOtherPylonsActive();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	bool m_IsPlayerFight = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	bool m_CanSpawn = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	bool m_IsInCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	int m_NrOfEnemies = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AISettings");
	float m_TimeToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	float m_MaxTimeToSpawn = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UArrowComponent* m_pGunPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UWeaponManagerComponent* m_pWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TArray<AActor*>  m_PatrolStarts;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	APawn* m_pPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TSubclassOf<AActor> m_pHealthOrbClass;

	TArray<AActor*> m_HealthPoints;

	TArray<AActor*> m_OtherPylons;

	TArray<AActor*> m_CurrentPatrolUnits;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
