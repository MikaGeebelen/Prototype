// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class AWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		TArray<TSubclassOf<AWeaponBase>> m_WeaponsToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
		TArray<AWeaponBase*> m_SpawnedWeapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
		AWeaponBase* m_pSelectedWeapon;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetWeaponLocation(const FVector& relativeLocation);
	void SetAttachmentComponent(USceneComponent* component);



	AWeaponBase* GetSelectedWeapon();

private:
	FVector m_WeaponRelativeLocation;
	USceneComponent* m_pComponentToAttach;

	void SetSelectedWeapon(int weaponIdx);
	void HideSelectedWeapon(bool hidden);
	void UpdateWeaponLocation();
};
