// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class AWeaponBase;
class UArrowComponent;

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
	void SetWeaponDropLocation(UArrowComponent* location);

	AWeaponBase* GetSelectedWeapon();
	void DropSelectedWeapon();
	void AddWeapon(AWeaponBase* weapon);
	void RemoveWeapon(AWeaponBase* weaponToRemove);

	void SetSelectedWeapon(int weaponIdx);

private:
	FVector m_WeaponRelativeLocation;
	USceneComponent* m_pComponentToAttach;
	UArrowComponent* m_pDropLocation;
	int m_CurrentWeaponIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Inventory")
		int m_InventorySlots = 2;

	void SetSelectedWeapon(AWeaponBase* weapon);
	void HideSelectedWeapon(bool hidden);
	void UpdateWeaponLocation();
};
