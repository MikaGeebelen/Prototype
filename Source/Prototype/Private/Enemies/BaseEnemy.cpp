// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ArrowComponent.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Weapons/WeaponManagerComponent.h"

#include "AIController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_pBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	m_pBody->SetupAttachment(RootComponent);
	m_pLookDir = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dir"));
	m_pLookDir->SetupAttachment(m_pBody);

	m_pGunPosition = CreateDefaultSubobject<UArrowComponent>("GunPos");
	m_pGunPosition->SetupAttachment(RootComponent);

	m_pHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	m_pWeapon = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("Weapon"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	m_pWeapon->SetWeaponLocation(m_pGunPosition->GetComponentLocation());
	//GetController()
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %d"),m_pHealth->GetHealth());
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_pHealth)
	{
		m_pHealth->TakeDamage(DamageAmount);

		if (m_pHealth->IsDead())
		{
			Destroy();
		}
	}

	return DamageAmount;
}
