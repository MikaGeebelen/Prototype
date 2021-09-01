// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/WeaponBase.h"

#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_WeaponsToSpawn.Num() > m_InventorySlots)
		m_InventorySlots = m_SpawnedWeapons.Num();

	for (TSubclassOf<AWeaponBase> weaponClass : m_WeaponsToSpawn)
	{
		AWeaponBase* spawnedWeapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(weaponClass));
		//spawnedWeapon->SetHidden(true);
		//spawnedWeapon->SetOwner(GetOwner());
		//m_SpawnedWeapons.Add(spawnedWeapon);
		AddWeapon(spawnedWeapon);
	}


	//SetSelectedWeapon(m_CurrentWeaponIndex);

}

void UWeaponManagerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	for (AWeaponBase* weapon : m_SpawnedWeapons)
	{
		if (weapon)
			weapon->Destroy();
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
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

void UWeaponManagerComponent::SetWeaponDropLocation(UArrowComponent* location)
{
	m_pDropLocation = location;
}

AWeaponBase* UWeaponManagerComponent::GetSelectedWeapon()
{
	return m_pSelectedWeapon;
}

void UWeaponManagerComponent::DropSelectedWeapon()
{
	if (!m_pDropLocation || !GetSelectedWeapon())
		return;

	FCollisionQueryParams params{};
	if (GetOwner())
		params.AddIgnoredActor(GetOwner());

	FHitResult hit{};
	FVector start = m_pDropLocation->GetComponentLocation();
	FVector end = m_pDropLocation->GetComponentLocation();
	end.Z -= 1000;

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_WorldDynamic, params))
	{
		AWeaponBase* weapon = GetSelectedWeapon();
		if (weapon)
		{
			FDetachmentTransformRules dettachRules
			{
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepWorld,
				true
			};

			if (m_pComponentToAttach)
			{
				weapon->DetachAllSceneComponents(m_pComponentToAttach, dettachRules);
			}
			else
			{
				weapon->DetachFromActor(dettachRules);
			}

			weapon->SetOwner(nullptr);
			weapon->ResetLocals();

			FVector position = hit.ImpactPoint;
			position.Z += 20;

			weapon->SetActorLocation(position);
			RemoveWeapon(weapon);
		}
	}
}

void UWeaponManagerComponent::AddWeapon(AWeaponBase* weapon)
{
	if (!weapon)
		return;

	m_SpawnedWeapons.Add(weapon);
	weapon->SetOwner(GetOwner());

	if (m_SpawnedWeapons.Num() > m_InventorySlots)
		DropSelectedWeapon();

	SetSelectedWeapon(m_SpawnedWeapons.Num() - 1);
	UpdateWeaponLocation();
}

void UWeaponManagerComponent::RemoveWeapon(AWeaponBase* weaponToRemove)
{
	m_SpawnedWeapons.Remove(weaponToRemove);

	if (weaponToRemove == m_pSelectedWeapon)
		m_pSelectedWeapon = nullptr;
}

void UWeaponManagerComponent::SetSelectedWeapon(int weaponIdx)
{
	if (weaponIdx < 0 || weaponIdx > m_SpawnedWeapons.Num() - 1 || m_SpawnedWeapons.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon not available"));
		return;
	}

	HideSelectedWeapon(true);
	m_pSelectedWeapon = m_SpawnedWeapons[weaponIdx];
	HideSelectedWeapon(false);

	FAttachmentTransformRules attachRules
	{
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		true
	};

	if (m_pComponentToAttach)
		m_pSelectedWeapon->AttachToComponent(m_pComponentToAttach, attachRules);
	else
		m_pSelectedWeapon->AttachToActor(GetOwner(), attachRules);

	UE_LOG(LogTemp, Warning, TEXT("Weapon selected"));
}

void UWeaponManagerComponent::SetSelectedWeapon(AWeaponBase* weapon)
{
	int i{ 0 };
	for (AWeaponBase* currentWeapon : m_SpawnedWeapons)
	{
		if (currentWeapon == weapon)
			break;
	}

	SetSelectedWeapon(i);
}

void UWeaponManagerComponent::HideSelectedWeapon(bool hidden)
{
	if (m_pSelectedWeapon)
		m_pSelectedWeapon->SetVisibility(!hidden);
}

void UWeaponManagerComponent::UpdateWeaponLocation()
{
	for (AWeaponBase* weapon : m_SpawnedWeapons)
	{
		weapon->SetActorLocation(m_WeaponRelativeLocation);
	}
}


// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (m_Once)
	{
		UpdateWeaponLocation();
		m_Once = false;
	}
	// ...
}

