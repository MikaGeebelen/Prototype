// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyManager.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

EnemyManager* EnemyManager::GetInstance()
{
	if (!m_pEnemyManager)
	{
		m_pEnemyManager = new EnemyManager();
	}
	return m_pEnemyManager;
}
