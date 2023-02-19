// Copyright Epic Games, Inc. All Rights Reserved.

#include "KitchenSinkCharacter.h"
#include "HudUi.h"
#include "KitchenSinkProjectile.h"
#include "PrjHud.h"
#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

//////////////////////////////////////////////////////////////////////////
// AKitchenSinkCharacter

AKitchenSinkCharacter::AKitchenSinkCharacter()
{
  // Character doesnt have a rifle at start
  bHasRifle = false;

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
}

void AKitchenSinkCharacter::BeginPlay()
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

  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController);
  playerController->SetInputMode(FInputModeGameOnly{});
  playerController->bShowMouseCursor = false;
}

//////////////////////////////////////////////////////////////////////////// Input

void AKitchenSinkCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
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
      MoveAction, ETriggerEvent::Triggered, this, &AKitchenSinkCharacter::move);

    // Looking
    EnhancedInputComponent->BindAction(
      LookAction, ETriggerEvent::Triggered, this, &AKitchenSinkCharacter::look);

    EnhancedInputComponent->BindAction(
      SettingsAction, ETriggerEvent::Triggered, this, &AKitchenSinkCharacter::settings);
  }
}

auto AKitchenSinkCharacter::move(const FInputActionValue &Value) -> void
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

auto AKitchenSinkCharacter::look(const FInputActionValue &Value) -> void
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

auto AKitchenSinkCharacter::settings() -> void
{
  LOG("settings");
  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController);
  auto hud = Cast<APrjHud>(playerController->GetHUD());
  CHECK_RET(hud);
  auto hudUi = hud->getHudUi();
  hudUi->toggleSettings();
}

void AKitchenSinkCharacter::SetHasRifle(bool bNewHasRifle)
{
  bHasRifle = bNewHasRifle;
}

bool AKitchenSinkCharacter::GetHasRifle()
{
  return bHasRifle;
}
