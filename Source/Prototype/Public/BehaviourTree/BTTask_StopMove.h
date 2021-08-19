// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopMove.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UBTTask_StopMove : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual void OnGameplayTaskActivated(UGameplayTask& task) override;
};
