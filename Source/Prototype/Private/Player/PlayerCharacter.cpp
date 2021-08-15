#include "Player/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind input to functions
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);

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
		UE_LOG(LogTemp, Warning, TEXT("Test"));
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

