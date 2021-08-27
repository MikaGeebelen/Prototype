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

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>("Player Camera");
	m_pCamera->SetupAttachment(m_pSpringArm);

	m_pWeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>("Weapon Manager");
	m_pWeaponManager->SetAttachmentComponent(RootComponent);
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
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind input to functions
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &APlayerCharacter::ShootWeapon);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Released, this, &APlayerCharacter::ReleaseWeapon);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &APlayerCharacter::VerticalMovement);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayerCharacter::HorizontalMovement);
	PlayerInputComponent->BindAxis("VerticalLook", this, &APlayerCharacter::VerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &APlayerCharacter::HorizontalLook);
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
		//AddMovementInput(GetActorRightVector(), axis);
		//FVector right = m_pCamera->GetRightVector();
		//right.Z = 0;
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


}

void APlayerCharacter::HorizontalLook(float axis)
{
	//AddControllerYawInput(axis);
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
	if (!m_pWeapon)
		return;

	FVector cameraForward = m_pSpringArm->GetForwardVector();
	FRotator rotation = cameraForward.ToOrientationRotator();
	m_pWeapon->SetActorRelativeRotation(FRotator{ rotation.Pitch - 10.f , 0, 0 });
}

void APlayerCharacter::ShootWeapon()
{
	AWeaponBase* selectedWeapon = m_pWeaponManager->GetSelectedWeapon();
	if (m_pWeapon != selectedWeapon)
	{
		m_pWeapon = selectedWeapon;
	}

	if (m_pWeapon)
	{
		LookInCameraDirection();
		m_pWeapon->ShootPrimary();
		m_pWeapon->IsFiring(true);
	}
}

void APlayerCharacter::ReleaseWeapon()
{
	if (m_pWeapon)
	{
		m_pWeapon->IsFiring(false);
	}
}

