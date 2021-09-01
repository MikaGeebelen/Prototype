#include "Player/PlayerCharacter.h"

//Engine
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

//Custom
#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Components/HealthComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pHealth = CreateDefaultSubobject<UHealthComponent>("Health");
	
	m_pGunPosition = CreateDefaultSubobject<UArrowComponent>("Gun Position");
	m_pGunPosition->SetupAttachment(RootComponent);

	m_pGunDropPosition = CreateDefaultSubobject<UArrowComponent>("Gun Drop Position");
	m_pGunDropPosition->SetupAttachment(RootComponent);

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>("Player Camera");
	m_pCamera->SetupAttachment(m_pSpringArm);

	m_pWeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>("Weapon Manager");
	m_pWeaponManager->SetAttachmentComponent(RootComponent);
	m_pWeaponManager->SetWeaponDropLocation(m_pGunDropPosition);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_pHealth->SetMaxHealth(m_PlayerHealth, true);
	m_OriginalRotation = m_pSpringArm->GetTargetRotation();
	m_DefaultCameraRotation = m_pCamera->GetRelativeRotation();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_UpdateWeaponPos && m_pWeaponManager)
	{
		m_pWeaponManager->SetWeaponLocation(m_pGunPosition->GetComponentLocation());
		m_UpdateWeaponPos = false;
	}

	if (m_IsWeaponFiring || m_FireWeapon)
	{
		LookInCameraDirection();
		UpdateWeaponRotation();
		ShootWeapon();
		m_FireWeapon = false;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind input to functions
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Released, this, &APlayerCharacter::ReleaseWeapon);
	PlayerInputComponent->BindAction("InventorySlot1", EInputEvent::IE_Pressed, this, &APlayerCharacter::SelectFirstWeapon);
	PlayerInputComponent->BindAction("InventorySlot2", EInputEvent::IE_Pressed, this, &APlayerCharacter::SelectSecondWeapon);
	PlayerInputComponent->BindAction("TestButton", EInputEvent::IE_Pressed, this, &APlayerCharacter::TestFunction);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &APlayerCharacter::VerticalMovement);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayerCharacter::HorizontalMovement);
	PlayerInputComponent->BindAxis("VerticalLook", this, &APlayerCharacter::VerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &APlayerCharacter::HorizontalLook);
}

UWeaponManagerComponent* APlayerCharacter::GetWeaponManager()
{
	return m_pWeaponManager;
}

void APlayerCharacter::PickUpWeapon(AWeaponBase* weapon)
{
	m_pWeaponManager->AddWeapon(weapon);
	m_UpdateWeaponPos = true;
}

float APlayerCharacter::TakeDamage(float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	if (m_pHealth)
	{
		m_pHealth->TakeDamage(DamageAmount);

		if (m_pHealth->IsDead())
		{
			RestartLevel();
		}
	}

	return DamageAmount;
}

void APlayerCharacter::RestartLevel()
{
	FName currentLevel = *UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), currentLevel);
}

//Movement
void APlayerCharacter::VerticalMovement(float axis)
{
	if (abs(axis) > 0.01f)
	{
		LookInCameraDirection();
		AddMovementInput(GetActorForwardVector(), axis);
	}
}

void APlayerCharacter::HorizontalMovement(float axis)
{
	if (abs(axis) > 0.01f)
	{
		LookInCameraDirection();
		AddMovementInput(GetActorRightVector(), axis);
	}
}

void APlayerCharacter::Jump()
{
	UCharacterMovementComponent* movementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (movementComp)
		movementComp->DoJump(false);
}

void APlayerCharacter::VerticalLook(float axis)
{
	FRotator rotation = UKismetMathLibrary::MakeRotator(0, axis, 0);
	m_pSpringArm->AddLocalRotation(rotation);

	FRotator currentRotation = m_pSpringArm->GetTargetRotation();
	float currentPitch = currentRotation.Pitch;
	currentPitch = UKismetMathLibrary::ClampAngle(currentPitch, m_MinVerticalAngle, m_MaxVerticalAngle);

	rotation = UKismetMathLibrary::MakeRotator(0, currentPitch, currentRotation.Yaw);
	m_pSpringArm->SetRelativeRotation(rotation);
	UpdateWeaponRotation();
}

void APlayerCharacter::HorizontalLook(float axis)
{
	FRotator rotation = UKismetMathLibrary::MakeRotator(0, 0, axis);
	m_pSpringArm->AddLocalRotation(rotation);

	FRotator currentRotation = m_pSpringArm->GetTargetRotation();
	currentRotation.Roll = 0;
	m_pSpringArm->SetRelativeRotation(currentRotation);
	UpdateWeaponRotation();
}

void APlayerCharacter::LookInCameraDirection()
{
	FVector cameraForward = m_pSpringArm->GetForwardVector();
	FVector playerForward = GetActorForwardVector();

	FVector direction = FVector::CrossProduct(cameraForward, playerForward);


	float angle = UKismetMathLibrary::Acos(cameraForward.CosineAngle2D(playerForward));
	angle = UKismetMathLibrary::RadiansToDegrees(angle);
	cameraForward.Z = 0;

	if (direction.Z > 0)
		angle *= -1;

	AController* controller = GetController();
	float originalYaw = m_OriginalRotation.Pitch;
	controller->SetControlRotation(FRotator{ 0, angle, 0 });
}

void APlayerCharacter::UpdateWeaponRotation()
{
	AWeaponBase* weapon = m_pWeaponManager->GetSelectedWeapon();
	if (!weapon)
		return;

	FVector cameraForward = m_pSpringArm->GetForwardVector();
	FRotator rotation = cameraForward.ToOrientationRotator();
	weapon->SetActorRelativeRotation(FRotator{ rotation.Pitch - 10.f , 0, 0 });
}

void APlayerCharacter::SelectFirstWeapon()
{
	m_pWeaponManager->SetSelectedWeapon(0);
}

void APlayerCharacter::SelectSecondWeapon()
{
	m_pWeaponManager->SetSelectedWeapon(1);
}

void APlayerCharacter::TestFunction()
{
	m_pWeaponManager->DropSelectedWeapon();
}

void APlayerCharacter::ShootWeapon()
{
	AWeaponBase* selectedWeapon = m_pWeaponManager->GetSelectedWeapon();
	//if (m_pWeapon != selectedWeapon)
	//{
	//	m_pWeapon = selectedWeapon;
	//}

	if (selectedWeapon)
	{
		LookInCameraDirection();
		selectedWeapon->ShootPrimary();
		m_IsWeaponFiring = true;
		selectedWeapon->IsFiring(m_IsWeaponFiring);
	}
}

void APlayerCharacter::FireWeapon()
{
	m_FireWeapon = true;
}

void APlayerCharacter::ReleaseWeapon()
{
	AWeaponBase* selectedWeapon = m_pWeaponManager->GetSelectedWeapon();
	if (selectedWeapon)
	{
		m_IsWeaponFiring = false;
		selectedWeapon->IsFiring(m_IsWeaponFiring);
	}
}

