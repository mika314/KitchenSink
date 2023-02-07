// (c) 2013 Mika Pi

#include "ShelterCharacter.h"
#include "../KitchenSinkProjectile.h"
#include "../TP_WeaponComponent.h"
#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

//////////////////////////////////////////////////////////////////////////
// AShelterCharacter

AShelterCharacter::AShelterCharacter()
  : weapon(CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("weapon")))
{
  // Character doesnt have a rifle at start
  bHasRifle = true;

  // Set size for collision capsule
  GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

  // Create a CameraComponent
  FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
  FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
  FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
  FirstPersonCameraComponent->bUsePawnControlRotation = true;

  // Create a mesh component that will be used when being viewed from a '1st person' view (when
  // controlling this pawn)
  Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
  Mesh1P->SetOnlyOwnerSee(true);
  Mesh1P->SetupAttachment(FirstPersonCameraComponent);
  Mesh1P->bCastDynamicShadow = false;
  Mesh1P->CastShadow = false;
  // Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
  Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
  weapon->AttachToComponent(Mesh1P, AttachmentRules, FName(TEXT("GripPoint")));
  weapon->Character = this;
}

void AShelterCharacter::BeginPlay()
{
  // Call the base class
  Super::BeginPlay();

  // Add Input Mapping Context
  if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
  {
    if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer()))
    {
      Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
  }
  // Set up action bindings
  if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
  {
    if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer()))
    {
      // Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action
      // when using touch input
      Subsystem->AddMappingContext(weapon->FireMappingContext, 1);
    }

    if (UEnhancedInputComponent *EnhancedInputComponent =
          Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
    {
      // Fire
      EnhancedInputComponent->BindAction(
        weapon->FireAction, ETriggerEvent::Triggered, weapon, &UTP_WeaponComponent::Fire);
    }
  }
  SetHasRifle(true);
}

//////////////////////////////////////////////////////////////////////////// Input

void AShelterCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
  // Set up action bindings
  if (UEnhancedInputComponent *EnhancedInputComponent =
        CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
  {
    // Jumping
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(
      JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

    // Moving
    EnhancedInputComponent->BindAction(
      MoveAction, ETriggerEvent::Triggered, this, &AShelterCharacter::Move);

    // Looking
    EnhancedInputComponent->BindAction(
      LookAction, ETriggerEvent::Triggered, this, &AShelterCharacter::Look);
  }
}

void AShelterCharacter::Move(const FInputActionValue &Value)
{
  // input is a Vector2D
  FVector2D MovementVector = Value.Get<FVector2D>();

  if (Controller != nullptr)
  {
    // add movement
    AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    AddMovementInput(GetActorRightVector(), MovementVector.X);
  }
}

void AShelterCharacter::Look(const FInputActionValue &Value)
{
  // input is a Vector2D
  FVector2D LookAxisVector = Value.Get<FVector2D>();

  if (Controller != nullptr)
  {
    // add yaw and pitch input to controller
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
  }
}

void AShelterCharacter::SetHasRifle(bool bNewHasRifle)
{
  bHasRifle = bNewHasRifle;
}

bool AShelterCharacter::GetHasRifle()
{
  return true;
}
