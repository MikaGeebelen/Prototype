// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

class APlayerController;
class APawn;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	APlayerController* m_pPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FVector m_PlayerPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	APawn* m_pEnemy;

	//Enemy specific
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "magic");
	float m_Range;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void ChasePlayer();
	virtual void LookAtPlayer();

	virtual void Tick(float DeltaSeconds) override;
};
