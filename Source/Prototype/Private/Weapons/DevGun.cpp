// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DevGun.h"

//Engine
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADevGun::ADevGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADevGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADevGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADevGun::ShootPrimary()
{
	FHitResult hit{};
	FCollisionQueryParams linetraceParams{};
	linetraceParams.AddIgnoredActor(this);

	if (m_pShootLocation && CanShoot())
	{
		if (m_PrintDebugHelp)
			UE_LOG(LogTemp, Warning, TEXT("DevGun has been shot"));
		
		FVector start{ m_pShootLocation->GetComponentLocation()};
		FVector end{ start + (m_MaxLineTraceDistance * GetActorForwardVector()) };

		if (m_DrawDebugLines)
			DrawDebugLine(GetWorld(), start, end, { 255, 0, 0 }, false, 10);

		if (m_ShootSoundEffect)
			UGameplayStatics::PlaySound2D(GetWorld(), m_ShootSoundEffect, 0.5f);

		ShootBullet();
		//if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Pawn, linetraceParams))
		//{
		//	auto hitActor = hit.Actor;

		//	if (!hitActor.IsValid())
		//		return;

		//	if (m_PrintDebugHelp)
		//	{
		//		FVector loc = hitActor->GetActorLocation();
		//		UE_LOG(LogTemp, Warning, TEXT("-------------Hit Info-------------"));
		//		UE_LOG(LogTemp, Warning, TEXT("Start Location %f %f %f | End Location %f %f %f"), start.X, start.Y, start.Z, end.X, end.Y, end.Z);
		//		UE_LOG(LogTemp, Warning, TEXT("Hit: %s, Location %f %f %f"), *hit.Actor->GetName(), loc.X, loc.Y, loc.Z);
		//		UE_LOG(LogTemp, Warning, TEXT("----------------------------------"));
		//	}

		//	if (hitActor != GetOwner())
		//	{
		//		if (m_PrintDebugHelp)
		//			UE_LOG(LogTemp, Warning, TEXT("%s has damaged %s"), *GetOwner()->GetName(), *hitActor->GetName());

		//		hitActor->TakeDamage(m_Damage, {}, {}, GetOwner());
		//	}
		//}

		ResetFireRateTime();
	}
}

