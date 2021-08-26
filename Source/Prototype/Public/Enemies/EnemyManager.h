// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyManager.generated.h"

class ABaseEnemy;
class ABaseEnemyController;
class APatrollingController;
class AWanderingController;

UCLASS()
class PROTOTYPE_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:
	AEnemyManager() = default;
	
	AActor* SpawnWanderingEnemy(FVector spawnLoc, float range = 200, float wanderRange = 200);
	AActor* SpawnPatrollingEnemy(FVector spawnLoc, TArray<AActor*> patrolArray, float range = 200);
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseEnemy> m_WanderBase;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWanderingController> m_WanderController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseEnemy> m_PatrolBase;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APatrollingController> m_PatrolController;
	
	
	TArray<ABaseEnemy*> m_EnemyPawns;
	TArray<ABaseEnemyController*> m_EnemyControllers;

};

