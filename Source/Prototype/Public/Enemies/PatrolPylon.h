// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPylon.generated.h"

UCLASS()
class PROTOTYPE_API APatrolPylon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPylon();

	virtual AActor* GetArriveLocation(FVector otherPylon);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TArray<AActor*>  m_PatrolStarts;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent*  m_pPillar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings");
	float m_MaxTimeToSpawn = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AISettings");
	float m_TimeToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIWanderSettings");
	bool m_CanSpawn = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual AActor* GetStartLocation(FVector otherPylon);
	
	virtual bool SendPatrol();

	virtual bool IsPatrolDead();


private:
	TArray<AActor*> m_OtherPylons;

	TArray<AActor*> m_CurrentPatrolUnits;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
