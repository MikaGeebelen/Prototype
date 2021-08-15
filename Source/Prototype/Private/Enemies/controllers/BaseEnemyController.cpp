// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/BaseEnemyController.h"

#include "Components/SceneComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "NavigationSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("start"));
	
	m_pPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	m_pEnemy = GetPawn();
}

void ABaseEnemyController::ChasePlayer()
{
	float distance{};
	FVector locE = m_pEnemy->GetActorLocation();
	//log position of target and self and distance between
	//UE_LOG(LogTemp, Warning, TEXT("enemy pos = %f %f %f"), locE.X, locE.Y, locE.Z);
	//UE_LOG(LogTemp, Warning, TEXT("player pos = %f %f %f"), m_PlayerPos.X,m_PlayerPos.Y, m_PlayerPos.Z);
	distance = FVector::Dist(m_pPlayer->GetActorLocation(), locE);
	//UE_LOG(LogTemp, Warning, TEXT("distance = %f"), distance);
	if (distance > m_Range)
	{
		MoveToActor(m_pPlayer,-1.0f,true,true);
	}
	else
	{
		StopMovement();
	}
}

void ABaseEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//update player pos

	ChasePlayer();
}
