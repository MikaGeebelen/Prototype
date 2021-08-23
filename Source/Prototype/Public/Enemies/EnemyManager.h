// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class BaseEnemy;
class BaseEnemyController;
/**
 * 
 */
class PROTOTYPE_API EnemyManager 
{
public:
	~EnemyManager();

	static EnemyManager* GetInstance();

	
	
private:
	EnemyManager() = default;

	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) = delete;
	EnemyManager& operator=(EnemyManager&&) = delete;
	
	static EnemyManager* m_pEnemyManager;
	
};
