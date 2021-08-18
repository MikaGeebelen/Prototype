// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DevGun.h"

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
	UE_LOG(LogTemp, Warning, TEXT("DevGun has been shot"));
}

