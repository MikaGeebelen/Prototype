

#include "Weapons/WeaponBase.h"

//Engine
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pRoot = CreateDefaultSubobject<UArrowComponent>("Root");
	m_pRoot->SetVisibility(false);
	SetRootComponent(m_pRoot);

	m_pShootLocation = CreateDefaultSubobject<UArrowComponent>("Shooting Location");
	m_pShootLocation->SetupAttachment(RootComponent);

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_pMesh->SetupAttachment(RootComponent);
	m_pMesh->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AWeaponBase::CanShoot()
{
	return m_FireRateTime >= 1 / m_FireRate;
}

bool AWeaponBase::IsFiring()
{
	return m_IsFiring;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_FireRateTime < 1 / m_FireRate)
		m_FireRateTime += DeltaTime;

	if (IsFiring() && m_CanHoldDownButton)
		ShootPrimary();
}

void AWeaponBase::ResetFireRateTime()
{
	m_FireRateTime = 0;
}

void AWeaponBase::IsFiring(bool firing)
{
	m_IsFiring = firing;
}

//void AWeaponBase::ShootPrimary()
//{
//	m_FireRateTime -= 1 / m_FireRate;
//}

