// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeGameInstance.h"
#include "Enemies/EnemyManager.h"
AEnemyManager* UPrototypeGameInstance::GetEnemyManager()
{
	if (!m_pEnemyManager)
	{
		AEnemyManager* temp = Cast<AEnemyManager>(GetWorld()->SpawnActor(m_EnemyMangerType));
		m_pEnemyManager = temp;
	}
	return m_pEnemyManager;
}
