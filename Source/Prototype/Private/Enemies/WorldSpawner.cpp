// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/WorldSpawner.h"
#include "PrototypeGameInstance.h"
#include "Enemies/EnemyManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AWorldSpawner::AWorldSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldSpawner::BeginPlay()
{
	Super::BeginPlay();
	m_pInstance = Cast<UPrototypeGameInstance>(GetGameInstance());
	m_pPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AWorldSpawner::CreateWave(int points)
{
	const int enemyTypes = 0;
	const float spawnRange = 2000.f;
	while (points >= 100)
	{
		int enemyToSpawn = UKismetMathLibrary::RandomInteger(enemyTypes);

		float spawnDistance = UKismetMathLibrary::RandomFloatInRange((spawnRange * 3) / 4, spawnRange);
		float spawnDir = UKismetMathLibrary::RandomFloatInRange(0.f,360.f);
		FVector dir{ spawnDistance,0,m_pPlayer->GetActorLocation().Z };
		dir.Normalize();
		dir = dir.RotateAngleAxis(spawnDir, FVector::UpVector);
		dir *= spawnDistance;
		
		switch (enemyToSpawn)
		{
		case 0:
			points -= 100;
			m_pInstance->GetEnemyManager()->SpawnWanderingEnemy(m_pPlayer->GetActorLocation() + dir,1000,2000);
			break;
		}
	}
}

// Called every frame
void AWorldSpawner::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (m_MaxTimePerWave < m_TimePassed)
	{
		m_TimePassed = 0;
		CreateWave((m_WaveLevel * m_WaveStrenghtIncrease) + m_WaveBaseStrenght);
		m_WaveLevel++;
	}
	else
	{
		m_TimePassed += deltaTime;
	}
}

