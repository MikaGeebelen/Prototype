// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bullet.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class PROTOTYPE_API ABullet : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABullet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider")
		UCapsuleComponent* m_pCapsuleCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* m_pMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		UFloatingPawnMovement* m_pMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetDirection(FVector direction);
	void SetDamage(float damage);

private:
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	float m_Damage = 1.f;


protected:
	FVector m_Direction = {1, 0, 0};
};
