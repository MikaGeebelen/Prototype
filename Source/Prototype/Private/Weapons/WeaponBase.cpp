

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

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_pMesh->SetupAttachment(RootComponent);
	m_pMesh->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::ShootPrimary()
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponBase has been shot"));
}

