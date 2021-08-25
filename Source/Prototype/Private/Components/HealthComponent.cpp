// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

//Engine
#include "Kismet/KismetMathLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	m_Health = m_MaxHealth;
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(int damageAmount)
{
	if (!IsDead())
	{
		m_Health -= damageAmount;
		ClampHealth();
	}
}

void UHealthComponent::AddHealth(int healthAmount)
{
	if (!IsDead())
	{
		m_Health += healthAmount;
		ClampHealth();
	}
}

void UHealthComponent::SetHealth(int healthAmount)
{
	m_Health = healthAmount;
	ClampHealth();
}

void UHealthComponent::SetMaxHealth(int healthAmount, bool restoreHealth)
{
	m_MaxHealth = healthAmount;
	if (restoreHealth)
		m_Health = m_MaxHealth;

	ClampHealth();
}


int UHealthComponent::GetHealth()
{
	return m_Health;
}

int UHealthComponent::GetMaxHealth()
{
	return m_MaxHealth;
}

bool UHealthComponent::IsDead()
{
	return m_Health <= 0;
}

void UHealthComponent::ClampHealth()
{
	m_Health = UKismetMathLibrary::Clamp(m_Health, 0, m_MaxHealth);
}

