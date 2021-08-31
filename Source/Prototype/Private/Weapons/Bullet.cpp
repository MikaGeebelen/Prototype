// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bullet.h"

//Engine
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pCapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collider");
	SetRootComponent(m_pCapsuleCollider);

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_pMesh->SetupAttachment(m_pCapsuleCollider);

	m_pMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	m_pCapsuleCollider->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pMovement)
	{
		m_pMovement->AddInputVector(m_Direction);
	}
}

// Called to bind functionality to input
void ABullet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABullet::SetDirection(FVector direction)
{
	m_Direction = direction;
}

void ABullet::SetDamage(float damage)
{
	m_Damage = damage;
}

void ABullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (GetController())
	{
		AActor* owner = GetController()->GetPawn();
		if (owner && OtherActor->GetOwner() != owner)
		{
			//if (GetController()->GetPawn() && OtherActor->GetOwner())
				//UE_LOG(LogTemp, Warning, TEXT("Owner: %s, Other Owner: %s"), *GetController()->GetPawn()->GetName(), *OtherActor->GetOwner()->GetName());
			OtherActor->TakeDamage(m_Damage, {}, {}, owner);
			Destroy();
		}
	}
}

