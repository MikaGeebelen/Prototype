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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	TArray<AActor*> m_Waypoints{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	int m_CurrentWaypoint = 0;

protected:
	
	virtual void Tick(float DeltaSeconds) override;
};
