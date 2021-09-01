// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldSpawner.generated.h"

class UPrototypeGameInstance;
class Apawn;
UCLASS()
class PROTOTYPE_API AWorldSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CreateWave(int points);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	APawn* m_pPlayer;
	
	UPrototypeGameInstance* m_pInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSettings");
	float m_MaxTimePerWave = 30.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpawnerSettings");
	float m_TimePassed = 0.f;
	int m_WaveLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSettings");
	int m_WaveBaseStrenght = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSettings");
	int m_WaveStrenghtIncrease = 50;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
