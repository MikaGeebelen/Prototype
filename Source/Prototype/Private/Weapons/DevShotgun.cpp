// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DevShotgun.h"

//Engine
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

ADevShotgun::ADevShotgun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADevShotgun::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADevShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADevShotgun::ShootPrimary()
{
	FHitResult hit{};
	FCollisionQueryParams linetraceParams{};
	linetraceParams.AddIgnoredActor(this);

	if (m_pShootLocation && CanShoot())
	{
		if (m_PrintDebugHelp)
			UE_LOG(LogTemp, Warning, TEXT("DevShotgun has been shot"));

		FVector start{ m_pShootLocation->GetComponentLocation() };
		FVector end{ start + (m_MaxLineTraceDistance * GetActorForwardVector()) };

		if (m_DrawDebugLines)
			DrawDebugLine(GetWorld(), start, end, { 255, 0, 0 }, false, 10);

		if (m_ShootSoundEffect)
			UGameplayStatics::PlaySound2D(GetWorld(), m_ShootSoundEffect);

		ShootBullet();
		ResetFireRateTime();
	}
}
