// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PrototypeGameInstance.generated.h"

class AEnemyManager;

UCLASS()
class PROTOTYPE_API UPrototypeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
        UFUNCTION(BlueprintPure,Category = "Singleton")
        AEnemyManager* GetEnemyManager();
private:
	UPROPERTY(VisibleAnywhere,Category = "Managers")
		AEnemyManager* m_pEnemyManager;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemyManager> m_EnemyMangerType;
};
