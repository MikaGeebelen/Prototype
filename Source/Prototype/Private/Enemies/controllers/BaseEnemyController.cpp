// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/BaseEnemyController.h"

#include "Components/SceneComponent.h"

void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetNavAgentLocation() != FVector(0))
	{
		if (m_pTarget)
		{
			MoveToLocation(FVector(m_pTarget->GetComponentLocation()));
		}
		else
		{
			MoveToLocation(FVector(-400,0,100));
		}
		
	}
}
