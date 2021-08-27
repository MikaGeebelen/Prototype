// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/controllers/BaseEnemyController.h"
#include "WanderingController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AWanderingController : public ABaseEnemyController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIWanderSettings");
	float m_WanderRange = 200;
	
	virtual bool WanderAroundSpawn() override;
	
private:

};
