// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/BaseEnemyController.h"

#include "Components/SceneComponent.h"
#include "Components/HealthComponent.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/WeaponBase.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "Enemies/BaseEnemy.h"
//ai
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/HealthComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseEnemyController::ABaseEnemyController()
{
	m_pData = NewObject<UBlackboardData>();
	m_pData->UpdatePersistentKey<UBlackboardKeyType_Object>(FName("Target"));

	m_pBTData = NewObject<UBehaviorTree>();

	m_pBlackBoard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	m_pPawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	m_pNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise"));
}


bool ABaseEnemyController::ChasePlayer() 
{
	if (FVector::Dist(m_pPlayer->GetActorLocation(), m_pEnemy->GetActorLocation()) > m_Range)
	{
		if (m_pBlackBoard->GetValueAsBool("HasPlayerInVision"))
		{
			MoveToActor(m_pPlayer);
			m_LastPlayerLoc = m_pPlayer->GetActorLocation();
		}
		else
		{
			MoveToLocation(m_LastPlayerLoc);
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

bool ABaseEnemyController::WanderAroundSpawn()
{
	return true;
}

bool ABaseEnemyController::Patrol()
{
	return true;
}

void ABaseEnemyController::Shoot()
{
	UWeaponManagerComponent* weapon = Cast<ABaseEnemy>(GetPawn())->m_pWeapon;
	if (weapon)
	{
		weapon->GetSelectedWeapon()->ShootPrimary();
	}
}

FVector ABaseEnemyController::GetDodgeLoc()
{
	//reposition around player
	float distance = FMath::RandRange((m_Range * 3)/4, m_Range);
	float degrees = FMath::RandRange(-90.0f, 90.0f);
	
	FVector newDir{ m_pEnemy->GetActorLocation() - m_pPlayer->GetActorLocation() };
	
	newDir.Normalize();
	newDir = newDir * distance;
	newDir = newDir.RotateAngleAxis(degrees, FVector::UpVector);
	
	return  newDir + m_pPlayer->GetActorLocation();
}


void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	m_pBlackBoard->SetValueAsObject("Player", m_pPlayer);
}

void ABaseEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn)
	{
		m_pEnemy = InPawn;
		m_SpawnLoc = m_pEnemy->GetActorLocation();
		m_WanderTarget = m_SpawnLoc;

		
		bool ok = m_pBlackBoard->InitializeBlackboard(*m_pData);
		if (!ok)
		{
			UE_LOG(LogTemp, Warning, TEXT("blackboard failed to initialize"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("blackboard initialized"));
		}

		UBrainComponent* brain = GetBrainComponent();
		RunBehaviorTree(m_pBTData);
		
		m_pBlackBoard->CacheBrainComponent(*brain);

		m_pBlackBoard->SetValueAsVector("WanderTarget", m_WanderTarget);
		m_pBlackBoard->SetValueAsBool("HasPlayerVision", false);
		m_pBlackBoard->SetValueAsObject("SelfActor", InPawn);
		m_pBlackBoard->SetValueAsBool("IsInRange", false);
		m_pBlackBoard->SetValueAsBool("IsDamaged", false);
		m_pBlackBoard->SetValueAsBool("IsWaiting", true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("nothing to posses"));
	}

}

void ABaseEnemyController::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	//update player pos
	if (m_pBlackBoard->GetValueAsBool("IsWaiting"))
	{
		if (m_CountDown < 0)
		{
			m_pBlackBoard->SetValueAsBool("IsWaiting", false);
		}
		else
		{
			m_CountDown -= deltaSeconds;
		}
	}
	
	if (m_pPlayer)
	{
		m_pBlackBoard->SetValueAsObject("Player", m_pPlayer);
		if (m_pPawnSensing->CouldSeePawn(m_pPlayer))
		{
			m_pBlackBoard->SetValueAsBool("HasPlayerInVision", true);
		}
		else
		{
			m_pBlackBoard->SetValueAsBool("HasPlayerInVision", false);
		}
	}
	if (m_pPlayer && m_pEnemy)
	{
		if (FVector::Dist(m_pPlayer->GetActorLocation(), m_pEnemy->GetActorLocation()) > m_Range)
		{
			m_pBlackBoard->SetValueAsBool("IsInRange", false);
		}
		else
		{
			m_pBlackBoard->SetValueAsBool("IsInRange", true);
		}
	}
	
	UHealthComponent* health = Cast<UHealthComponent>(m_pEnemy->GetComponentByClass(UHealthComponent::StaticClass()));
	if (health)
	{
		if (health->GetMaxHealth() > health->GetHealth())
		{
			m_pBlackBoard->SetValueAsBool("IsDamaged", true);
		}
	}
}
