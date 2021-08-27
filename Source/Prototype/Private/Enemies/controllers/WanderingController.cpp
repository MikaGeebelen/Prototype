// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/WanderingController.h"

//ai
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool AWanderingController::WanderAroundSpawn()
{
	if (m_pBlackBoard->IsValidKey(m_pBlackBoard->GetKeyID("WanderTarget")))
	{
		float distance = FMath::RandRange(0.0f, m_WanderRange);
		float degrees = FMath::RandRange(0.0f, 360.0f);

		FVector dir{ distance,0,0 };
		//UE_LOG(LogTemp, Warning, TEXT("dir:%f,%f,%f"), dir.X, dir.Y, dir.Z);
		dir = dir.RotateAngleAxis(degrees, FVector::UpVector);
		//UE_LOG(LogTemp, Warning, TEXT("dir after rot:%f,%f,%f"), dir.X, dir.Y, dir.Z);

		m_WanderTarget = m_SpawnLoc + dir;
		MoveToLocation(m_WanderTarget);
		m_pBlackBoard->SetValueAsVector("WanderTarget", m_WanderTarget);
		return true;
	}
	else
	{
		return false;
	}
}
