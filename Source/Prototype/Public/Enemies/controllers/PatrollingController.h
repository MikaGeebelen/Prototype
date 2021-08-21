// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/controllers/BaseEnemyController.h"
#include "PatrollingController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APatrollingController : public ABaseEnemyController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPatrolSettings");
	TArray<AActor*> m_Waypoints{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AIPatrolSettings");
	int m_CurrentWaypoint = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPatrolSettings");
	FVector m_PatrolTarget;
	
	virtual bool Patrol() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};
