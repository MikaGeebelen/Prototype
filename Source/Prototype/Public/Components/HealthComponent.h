// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void TakeDamage(int damageAmount);
	UFUNCTION(BlueprintCallable)
		void AddHealth(int healthAmount);

	UFUNCTION(BlueprintCallable)
		void SetHealth(int healthAmount);
	UFUNCTION(BlueprintCallable)
		void SetMaxHealth(int healthAmount, bool restoreHealth = false);

	UFUNCTION(BlueprintCallable)
		int GetHealth();
	UFUNCTION(BlueprintCallable)
		int GetMaxHealth();

	UFUNCTION(BlueprintCallable)
		bool IsDead();

private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
		int m_Health;
	UPROPERTY(EditAnywhere, Category = "Health")
		int m_MaxHealth = 10;

	void ClampHealth();
};
