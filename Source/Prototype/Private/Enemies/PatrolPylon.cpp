// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/PatrolPylon.h"
#include "PrototypeGameInstance.h"
#include "Enemies/EnemyManager.h"
#include "Engine/TargetPoint.h"

#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"

// Sets default values
APatrolPylon::APatrolPylon()
{
	PrimaryActorTick.bCanEverTick = true;
	m_pPillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	SetRootComponent(m_pPillar);
}

AActor* APatrolPylon::GetArriveLocation(FVector otherPylon)
{
	float shortestRange = FLT_MAX;
	AActor* pArrive = nullptr;
	for (AActor* ArrivePoint : m_PatrolStarts)
	{
		float distance = FVector::Dist(otherPylon, ArrivePoint->GetActorLocation());
		if (shortestRange > distance)
		{
			shortestRange = distance;
			pArrive = ArrivePoint;
		}
	}

	return pArrive;
}

// Called when the game starts or when spawned
void APatrolPylon::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPylon::StaticClass(), m_OtherPylons);

	m_OtherPylons.Remove(this);
	
	FVector dir{ 200,0,0 };

	const int patrolPoints = 4;

	for (int i{}; i < patrolPoints; i++)
	{
		m_PatrolStarts.Push(GetWorld()->SpawnActor<ATargetPoint>());
		m_PatrolStarts[i]->SetActorLocation(GetActorLocation() + dir);
		dir = dir.RotateAngleAxis(360 / patrolPoints, FVector::UpVector);
	}
}

AActor* APatrolPylon::GetStartLocation(FVector otherPylon)
{
	float shortestRange = FLT_MAX;
	AActor* pStart = nullptr;
	for (AActor* startPoint : m_PatrolStarts)
	{
		float distance = FVector::Dist(otherPylon, startPoint->GetActorLocation());
		if (shortestRange > distance)
		{
			shortestRange = distance;
			pStart = startPoint;
		}
	}

	return pStart;
}

bool APatrolPylon::SendPatrol()
{
	TArray<AActor*> patrolPoints;

	const int maxTries = 100;
	int tries = 0;
	
	for (int i{ rand() % m_OtherPylons.Num() }; i< m_OtherPylons.Num(); i = rand() % m_OtherPylons.Num())
	{
		if (m_OtherPylons[i])
		{
			patrolPoints.Push(Cast<APatrolPylon>(m_OtherPylons[i])->GetArriveLocation(GetActorLocation()));
			patrolPoints.Push(GetStartLocation(m_OtherPylons[i]->GetActorLocation()));
			UPrototypeGameInstance* instance = Cast<UPrototypeGameInstance>(GetGameInstance());
			m_CurrentPatrolUnits.Push(instance->GetEnemyManager()->SpawnPatrollingEnemy(patrolPoints[1]->GetActorLocation(), patrolPoints, 400));
			return true;
		}

		tries++;
		if (tries >= maxTries)
		{
			return false;
		}
	}
	return false;
}

bool APatrolPylon::IsPatrolDead()
{
	for (AActor* unit : m_CurrentPatrolUnits)
	{
		if (unit)
		{
			return false;
		}
	}
	m_CurrentPatrolUnits.Empty();
	return true;
}

// Called every frame
void APatrolPylon::Tick(float deltaTime)
{
	Super::Tick(deltaTime);


	m_CanSpawn = IsPatrolDead();

	
	if (m_CanSpawn)
	{
		m_TimeToSpawn += deltaTime;
		if (m_TimeToSpawn >= m_MaxTimeToSpawn)
		{
			m_TimeToSpawn = 0;
			if (!SendPatrol())
			{
				m_CanSpawn = false;
			}
		}
	}
}

