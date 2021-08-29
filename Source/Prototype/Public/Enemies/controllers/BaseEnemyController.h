// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

class APawn;
class UPawnSensingComponent;
class UPawnNoiseEmitterComponent;
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
	UPawnNoiseEmitterComponent* m_pNoiseEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	UBehaviorTree* m_pBTData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	float m_Range = 200;
	
	//public funtions used as blueprint nodes
	
	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual bool ChasePlayer();

	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual bool WanderAroundSpawn();

	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual bool Patrol();

	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual void Shoot();

	UFUNCTION(BlueprintCallable, Category = "AITask")
	virtual FVector GetDodgeLoc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = "AIWanderSettings");
	FVector m_SpawnLoc;

	UPROPERTY(VisibleAnywhere, Category = "AIWanderSettings");
	FVector m_WanderTarget;
};
