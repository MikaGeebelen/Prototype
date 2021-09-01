// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/PatrolPylon.h"
#include "PrototypeGameInstance.h"
#include "Enemies/EnemyManager.h"
#include "Engine/TargetPoint.h"

#include "Components/ArrowComponent.h"
#include "Weapons/WeaponManagerComponent.h"

#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "NavModifierComponent.h"

#include "NavAreas/NavArea_Null.h"
#include "NavAreas/NavArea_Default.h"

#include "Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/WeaponBase.h"

// Sets default values
APatrolPylon::APatrolPylon()
{
	PrimaryActorTick.bCanEverTick = true;
	m_pPillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	SetRootComponent(m_pPillar);

	m_pGunPos = CreateDefaultSubobject<UArrowComponent>("GunPos");
	m_pGunPos->SetupAttachment(RootComponent);
	m_pWeapon = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("Weapon"));
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

	m_pPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	m_OtherPylons.Remove(this);
	
	FVector dir{ 300,0,0 };

	const int patrolPoints = 4;

	for (int i{}; i < patrolPoints; i++)
	{
		m_PatrolStarts.Push(GetWorld()->SpawnActor<ATargetPoint>());
		m_PatrolStarts[i]->SetActorLocation(GetActorLocation() + dir);
		dir = dir.RotateAngleAxis(360 / patrolPoints, FVector::UpVector);
	}

	m_pWeapon->SetWeaponLocation(m_pGunPos->GetComponentLocation());
	m_pWeapon->SetAttachmentComponent(m_pGunPos);


	TArray<UArrowComponent*> spawnLocs{};
	for (UActorComponent* healthLoc : GetComponentsByClass(UArrowComponent::StaticClass()))
	{
		if (healthLoc->ComponentHasTag("Health"))
		{
			spawnLocs.Add(Cast<UArrowComponent>(healthLoc));
		}
	}

	FActorSpawnParameters params{};
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (auto SpawnLoc : spawnLocs)
	{
		FVector vector = SpawnLoc->GetComponentLocation();
		FRotator rotator = SpawnLoc->GetComponentRotation();
		m_HealthPoints.Add(GetWorld()->SpawnActor(m_pHealthOrbClass, &vector, &rotator, params));
	}
	//required
	ToggleGates(true);
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

	AreOtherPylonsActive();
	
	if (m_OtherPylons.Num() < 1)
	{
		for (int i{ rand() % m_OtherPylons.Num() }; i < m_OtherPylons.Num(); i = rand() % m_OtherPylons.Num())
		{
			if (m_OtherPylons[i])
			{
				patrolPoints.Push(Cast<APatrolPylon>(m_OtherPylons[i])->GetArriveLocation(GetActorLocation()));
				patrolPoints.Push(GetStartLocation(m_OtherPylons[i]->GetActorLocation()));
				UPrototypeGameInstance* instance = Cast<UPrototypeGameInstance>(GetGameInstance());
				for (int j{}; j < m_NrOfEnemies; j++)
				{
					m_CurrentPatrolUnits.Push(instance->GetEnemyManager()->SpawnPatrollingEnemy(patrolPoints[1]->GetActorLocation(), patrolPoints, 1000));
				}
				return true;
			}

			tries++;
			if (tries >= maxTries)
			{
				return false;
			}
		}
	}
	return false;
}

bool APatrolPylon::IsPatrolDead()
{
	for (AActor* unit : m_CurrentPatrolUnits)
	{
		if (IsValid(unit))
		{
			return false;
		}
	}
	m_CurrentPatrolUnits.Empty();
	return true;
}

void APatrolPylon::Destroyed()
{
	ToggleGates(true);
}

void APatrolPylon::ToggleGates(bool canPass)
{
	FAttachmentTransformRules attachRules
	{
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		true
	};

	if (m_pWeapon && m_pWeapon->GetSelectedWeapon())
	{
		m_pWeapon->SetWeaponLocation(m_pGunPos->GetComponentLocation());
		m_pWeapon->GetSelectedWeapon()->AttachToComponent(m_pGunPos, attachRules);
		m_pWeapon->GetSelectedWeapon()->AddActorLocalOffset({ 100,0,0 });
	}
	
	for (AActor* pEntrance : m_Entrances)
	{
		//UNavModifierComponent* pNav = Cast<UNavModifierComponent>(pEntrance->GetComponentByClass(UNavModifierComponent::StaticClass()));
		//if (pNav)
		//{
		//	pNav->SetAreaClass(UNavArea_Default::StaticClass());
		//}

		USceneComponent* pRoot = pEntrance->GetRootComponent();
		UStaticMeshComponent* pMesh = Cast<UStaticMeshComponent>(pRoot);
		if (pMesh)
		{
			if (canPass)
			{
				pMesh->SetCollisionProfileName({ "OverlapAll" });
			}
			else
			{
				pMesh->SetCollisionProfileName({ "BlockAll" });
			}
		}
	}
}

void APatrolPylon::ShootPlayer()
{
	FVector playerDir = m_pPlayer->GetActorLocation() - GetActorLocation();
	playerDir.Z = 0;
	playerDir.Normalize();

	m_pGunPos->SetRelativeRotation(playerDir.ToOrientationRotator());
	m_pWeapon->GetSelectedWeapon()->ShootPrimary();
}

void APatrolPylon::SpawnDefenses()
{
	UPrototypeGameInstance* instance = Cast<UPrototypeGameInstance>(GetGameInstance());
	instance->GetEnemyManager()->SpawnWanderingEnemy(GetActorLocation() + FVector{ 250,0,0} , 1000 );
	instance->GetEnemyManager()->SpawnWanderingEnemy(GetActorLocation() + FVector{ -250,0,0}, 1000 );
}

void APatrolPylon::AreOtherPylonsActive()
{
	TArray<AActor*> destroyedPylons{};
	for (AActor* OtherPylon : m_OtherPylons)
	{
		if (!IsValid(OtherPylon))
		{
			destroyedPylons.Add(OtherPylon);
		}
	}

	for (AActor* OtherPylon : destroyedPylons)
	{
		m_OtherPylons.Remove(OtherPylon);
	}
}

// Called every frame
void APatrolPylon::Tick(float deltaTime)
{
	Super::Tick(deltaTime);


	if (m_IsPlayerFight)
	{
		if (!m_IsInCombat)
		{
			ToggleGates(false);
			m_IsInCombat = true;
		}

		ShootPlayer();

		TArray<AActor*> toBeRemoved{};

		for (AActor* pHealthPoint : m_HealthPoints)
		{
			UHealthComponent* pHealth = Cast<UHealthComponent>(pHealthPoint->GetComponentByClass(UHealthComponent::StaticClass()));
			if (pHealth->IsDead())
			{
				SpawnDefenses();
				//m_HealthPoints.Remove(pHealthPoint);
				toBeRemoved.Add(pHealthPoint);
				pHealthPoint->Destroy();

			}
		}

		for (AActor* pHealthPoint : toBeRemoved)
		{
			m_HealthPoints.Remove(pHealthPoint);
		}
		toBeRemoved.Empty();

		if (m_HealthPoints.Num() == 0)
		{
			Destroy();
		}
	}
	else
	{
		//spawn patrols
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
}


