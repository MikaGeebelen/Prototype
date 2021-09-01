

#include "Weapons/WeaponBase.h"

//Engine
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

//Custom
#include "Player/PlayerCharacter.h"
#include "Weapons/Bullet.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pCollider = CreateDefaultSubobject<UCapsuleComponent>("Root");
	SetRootComponent(m_pCollider);

	m_pShootLocation = CreateDefaultSubobject<UArrowComponent>("Shooting Location");
	m_pShootLocation->SetupAttachment(RootComponent);

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_pMesh->SetCollisionProfileName("NoCollision");
	m_pMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	m_pCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::BeginOverlap);
}

bool AWeaponBase::CanShoot()
{
	return m_FireRateTime >= 1 / m_FireRate;
}

bool AWeaponBase::IsPickUp()
{
	if (GetOwner())
	{
		m_pCollider->SetGenerateOverlapEvents(false);
		return false;
	}

	m_pCollider->SetGenerateOverlapEvents(true);
	return true;
}

bool AWeaponBase::IsFiring()
{
	return m_IsFiring;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPickUp())
	{
		AddActorLocalRotation(FRotator{ 0, DeltaTime * m_RotationSpeed, 0 });
	}
	else
	{
		if (m_FireRateTime < 1 / m_FireRate)
			m_FireRateTime += DeltaTime;

		if (IsFiring() && m_CanHoldDownButton)
			ShootPrimary();
	}
}

void AWeaponBase::ResetFireRateTime()
{
	m_FireRateTime = 0;
}

void AWeaponBase::ShootBullet()
{
	ShootBullet(m_pShootLocation->GetForwardVector());
}

void AWeaponBase::ShootBullet(const FVector& direction)
{
	if (m_BulletType && m_pShootLocation)
	{
		FActorSpawnParameters params{};
		params.Owner = this;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABullet* bullet = Cast<ABullet>(GetWorld()->SpawnActor(m_BulletType, 0, 0, params));
		if (bullet)
		{
			bullet->SetActorLocation(m_pShootLocation->GetComponentLocation());
			bullet->SetActorRotation(m_pShootLocation->GetComponentRotation());
			bullet->SetDirection(direction);
			bullet->SetDamage(m_Damage);

			if (GetOwner())
			{
				bullet->SetOwner(GetOwner());
			}
			bullet->SetLifeSpan(m_BulletMaxLifeSpawn);
		}
	}
}

void AWeaponBase::IsFiring(bool firing)
{
	m_IsFiring = firing;
}

void AWeaponBase::ResetLocals()
{
	SetActorRelativeLocation({0, 0, 0});
	SetActorRelativeRotation({0, 0, 0});
}

void AWeaponBase::SetVisibility(bool visible)
{
	m_pMesh->SetVisibility(visible);
}

void AWeaponBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex,
                               bool bFromSweep,
                               const FHitResult& SweepResult)
{
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsPickUp() && OtherActor == playerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon has been picked up!"));
		APlayerCharacter* player = Cast<APlayerCharacter>(playerPawn);
		if (player)
		{
			SetActorLocation(player->GetActorLocation());
			player->PickUpWeapon(this);
		}
	}
}

