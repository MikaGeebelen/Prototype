// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEnemy.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;


UCLASS()
class PROTOTYPE_API ABaseEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UCapsuleComponent* m_pCapsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* m_pBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* m_pLookDir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UFloatingPawnMovement* m_pMoveComp;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
