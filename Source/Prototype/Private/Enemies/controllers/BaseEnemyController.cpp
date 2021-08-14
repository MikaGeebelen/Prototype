// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/BaseEnemyController.h"

#include "Components/SceneComponent.h"


#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	m_pPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	m_PlayerPos = m_pPlayer->GetFocalLocation();
	m_pEnemy = GetPawn();
}

void ABaseEnemyController::ChasePlayer()
{
	if (FVector::Dist(m_PlayerPos, m_pEnemy->GetActorLocation()) > m_Range)
	{
		if (IsMoveInputIgnored())
		{
			MoveToLocation(m_PlayerPos);
			SetIgnoreMoveInput(true);
		}

	}
	else
	{
		if (!IsMoveInputIgnored())
		{
			SetIgnoreMoveInput(false);
			StopMovement();
		}
	}
}

void ABaseEnemyController::LookAtPlayer()
{
	FVector lookDir = m_PlayerPos - m_pEnemy->GetActorLocation();
	FRotator rotator = FRotationMatrix::MakeFromX(lookDir).Rotator();
	FHitResult result{};
	GetPawn()->SetActorRotation(rotator);
}

void ABaseEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//update player pos
	m_PlayerPos = m_pPlayer->GetFocalLocation();

	ChasePlayer();
	LookAtPlayer();
}
