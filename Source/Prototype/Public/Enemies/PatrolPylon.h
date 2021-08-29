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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	bool m_IsPlayerFight = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	bool m_CanSpawn = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AISettings");
	float m_TimeToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	float m_MaxTimeToSpawn = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	TArray<UArrowComponent*> m_HealhtOrbSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	TArray<UArrowComponent*> m_WeaponSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UWeaponManagerComponent* m_pWeapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TArray<AActor*>  m_PatrolStarts;

private:
	
	TArray<AActor*> m_HealthPoints;
	
	TArray<AActor*> m_OtherPylons;

	TArray<AActor*> m_CurrentPatrolUnits;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
