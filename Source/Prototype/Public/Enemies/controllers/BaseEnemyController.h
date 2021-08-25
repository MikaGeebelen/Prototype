// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

class APawn;
class UPawnSensingComponent;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	ABaseEnemyController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	APawn* m_pPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	APawn* m_pEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FVector m_LastPlayerLoc;

	//Enemy specific
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AISettings");
	UBlackboardComponent* m_pBlackBoard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UBlackboardData* m_pData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UPawnSensingComponent* m_pPawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UBehaviorTree* m_pBTData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	float m_Range = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIWanderSettings");
	float m_WanderRange = 200;

	//public funtions used as blueprint nodes
	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual bool ChasePlayer();

	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual bool WanderAroundSpawn();

	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual bool Patrol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "AIWanderSettings");
	FVector m_SpawnLoc;

	UPROPERTY(VisibleAnywhere, Category = "AIWanderSettings");
	FVector m_WanderTarget;
};
