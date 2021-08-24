// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyManager.h"

#include "Enemies/BaseEnemy.h"

#include "Enemies/controllers/BaseEnemyController.h"
#include "Enemies/controllers/PatrollingController.h"


AActor* AEnemyManager::SpawnWanderingEnemy(FVector spawnLoc, float range, float wanderRange)
{
	ABaseEnemyController* pController = Cast<ABaseEnemyController>(GetWorld()->SpawnActor(m_WanderController));
	ABaseEnemy* pPawn = Cast<ABaseEnemy>(GetWorld()->SpawnActor(m_WanderBase));
	pPawn->SetActorLocation(spawnLoc);

	pController->Possess(pPawn);
	pController->m_WanderRange = wanderRange;
	pController->m_Range = range;

	m_EnemyControllers.Push(pController);
	m_EnemyPawns.Push(pPawn);

	return pPawn;
}

AActor* AEnemyManager::SpawnPatrollingEnemy(FVector spawnLoc, TArray<AActor*> patrolArray, float range)
{
	APatrollingController* pController = Cast<APatrollingController>(GetWorld()->SpawnActor(m_PatrolController));
	ABaseEnemy* pPawn = Cast<ABaseEnemy>(GetWorld()->SpawnActor(m_PatrolBase));
	pPawn->SetActorLocation(spawnLoc);
	
	pController->Possess(pPawn);
	pController->m_Waypoints = patrolArray;
	pController->m_Range = range;

	m_EnemyControllers.Push(pController);
	m_EnemyPawns.Push(pPawn);

	return pPawn;
}
