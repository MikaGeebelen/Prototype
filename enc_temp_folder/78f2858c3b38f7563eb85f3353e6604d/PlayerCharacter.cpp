#include "Player/PlayerCharacter.h"

//Engine
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

//Custom
#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponManagerComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pGunPosition = CreateDefaultSubobject<UArrowComponent>("Gun Position");
	m_pGunPosition->SetupAttachment(RootComponent);

	m_pWeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>("Weapon Manager");

	m_pCamera = CreateDefaultSubobject<UCameraComponent>("Player Camera");
	m_pCamera->SetupAttachment(RootComponent);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_UpdateWeaponPos && m_pWeaponManager)
	{
		m_pWeaponManager->SetAttachmentComponent(m_pCamera);
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

	PlayerInputComponent->BindAxis("VerticalMovement", this, &APlayerCharacter::VerticalMovement);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayerCharacter::HorizontalMovement);
	PlayerInputComponent->BindAxis("VerticalLook", this, &APlayerCharacter::VerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &APlayerCharacter::HorizontalLook);
}

//Movement
void APlayerCharacter::VerticalMovement(float axis)
{
	if (abs(axis) > 0.01f)
	{
		AddMovementInput(GetActorForwardVector(), axis);
	}
}

void APlayerCharacter::HorizontalMovement(float axis)
{
	if (abs(axis) > 0.01f)
	{
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
	AddControllerPitchInput(axis);
}

void APlayerCharacter::HorizontalLook(float axis)
{
	AddControllerYawInput(axis);
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
		m_pWeapon->ShootPrimary();
	}
}

