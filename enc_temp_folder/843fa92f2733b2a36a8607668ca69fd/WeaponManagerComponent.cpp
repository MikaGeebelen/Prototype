// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/WeaponBase.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<AWeaponBase> weaponClass : m_WeaponsToSpawn)
	{
		AWeaponBase* spawnedWeapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(weaponClass));
		spawnedWeapon->SetHidden(true);
		m_SpawnedWeapons.Add(spawnedWeapon);
	}

	SetSelectedWeapon(0);
	UpdateWeaponLocation();
}

void UWeaponManagerComponent::SetWeaponLocation(const FVector& relativeLocation)
{
	m_WeaponRelativeLocation = relativeLocation;
	UpdateWeaponLocation();
}

void UWeaponManagerComponent::SetAttachmentComponent(USceneComponent* component)
{
	m_pComponentToAttach = component;
}

AWeaponBase* UWeaponManagerComponent::GetSelectedWeapon()
{
	return m_pSelectedWeapon;
}

void UWeaponManagerComponent::SetSelectedWeapon(int weaponIdx)
{
	if (weaponIdx < 0 || weaponIdx > m_SpawnedWeapons.Num() - 1 || m_SpawnedWeapons.Num() == 0)
		return;

	HideSelectedWeapon(true);
	m_pSelectedWeapon = m_SpawnedWeapons[weaponIdx];
	HideSelectedWeapon(false);

	FAttachmentTransformRules attachRules
	{
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		false
	};

	if (m_pComponentToAttach)
		m_pSelectedWeapon->AttachToComponent(m_pComponentToAttach, attachRules);
	else
		m_pSelectedWeapon->AttachToActor(GetOwner(), attachRules);
}

void UWeaponManagerComponent::HideSelectedWeapon(bool hidden)
{
	if (m_pSelectedWeapon)
		m_pSelectedWeapon->SetHidden(hidden);
}

void UWeaponManagerComponent::UpdateWeaponLocation()
{
	for (AWeaponBase* weapon : m_SpawnedWeapons)
	{
		weapon->SetActorLocation(m_WeaponRelativeLocation);
	}
}


// Called every frame
//void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

