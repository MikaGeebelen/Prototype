// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"


#include "GameFramework/FloatingPawnMovement.h"


#include "AIController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(m_pCapsule);
	m_pBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	m_pBody->SetupAttachment(m_pCapsule);
	m_pLookDir = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("dir"));
	m_pLookDir->SetupAttachment(m_pCapsule);
	
	m_pMoveComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	m_pMoveComp->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

UPawnMovementComponent* ABaseEnemy::GetMovementComponent() const
{
	return m_pMoveComp;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


