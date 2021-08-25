// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/PatrollingController.h"

//ai
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool APatrollingController::Patrol()
{
	if (m_Waypoints.Num() > 0)
	{
		m_CurrentWaypoint++;
		if (!(m_CurrentWaypoint < m_Waypoints.Num()))
		{
			m_CurrentWaypoint = 0;
		}

		m_PatrolTarget = m_Waypoints[m_CurrentWaypoint]->GetActorLocation();
		
		MoveToLocation(m_PatrolTarget);
		m_pBlackBoard->SetValueAsVector("PatrolTarget", m_PatrolTarget);
		return true;
	}
	else
	{
		return false;
	}
}

void APatrollingController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (m_Waypoints.Num() > 0)
	{
		if (m_Waypoints[0])
		{
			m_PatrolTarget = m_Waypoints[0]->GetActorLocation();
		}
	}
	m_pBlackBoard->SetValueAsVector("PatrolTarget", m_PatrolTarget);
}
