// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/controllers/BaseEnemyController.h"

#include "Components/SceneComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

//ai
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseEnemyController::ABaseEnemyController()
{
	m_pData = NewObject<UBlackboardData>();
	m_pData->UpdatePersistentKey<UBlackboardKeyType_Object>(FName("Target"));

	m_pBTData = NewObject<UBehaviorTree>();

	m_pBlackBoard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	m_pPawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	m_pPawnSensing->SetPeripheralVisionAngle(160);
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

		m_pBlackBoard->SetValueAsBool("HasPlayerVision", false);
		m_pBlackBoard->SetValueAsObject("SelfActor", InPawn);
		m_pBlackBoard->SetValueAsBool("IsInRange", false);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("nothing to posses"));
	}

}

void ABaseEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//update player pos
	if (m_pPawnSensing->CouldSeePawn(m_pPlayer))
	{
		m_pBlackBoard->SetValueAsBool("HasPlayerInVision", true);
		
	}
	else
	{
		m_pBlackBoard->SetValueAsBool("HasPlayerInVision", false);
	}	

	if (FVector::Dist(m_pPlayer->GetActorLocation(),m_pEnemy->GetActorLocation()) > m_Range)
	{
		m_pBlackBoard->SetValueAsBool("IsInRange", false);
	}
	else
	{
		m_pBlackBoard->SetValueAsBool("IsInRange", true);
	}
}
