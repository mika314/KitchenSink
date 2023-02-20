// Copyright Epic Games, Inc. All Rights Reserved.

#include "KitchenSinkCharacter.h"
#include "HudUi.h"
#include "KitchenSinkProjectile.h"
#include "PrjHud.h"
#include "Settings.h"
#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

AKitchenSinkCharacter::AKitchenSinkCharacter()
{
  bHasRifle = false;
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
  Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

auto AKitchenSinkCharacter::BeginPlay() -> void
{
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

auto AKitchenSinkCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
  -> void
{
  auto inputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
  CHECK_RET(inputComp);
  inputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
  inputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
  inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKitchenSinkCharacter::move);
  inputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKitchenSinkCharacter::look);
  inputComp->BindAction(
    SettingsAction, ETriggerEvent::Triggered, this, &AKitchenSinkCharacter::settings);
  updateMouseSensitivity();
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
    AddControllerYawInput(LookAxisVector.X * mouseSensitivity);
    AddControllerPitchInput(LookAxisVector.Y * mouseSensitivity);
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

auto AKitchenSinkCharacter::SetHasRifle(bool bNewHasRifle) -> void
{
  bHasRifle = bNewHasRifle;
}

auto AKitchenSinkCharacter::GetHasRifle() -> bool
{
  return bHasRifle;
}

auto AKitchenSinkCharacter::updateMouseSensitivity() -> void
{
  CHECK_RET(LookAction);
  auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
  if (!settings)
    settings = NewObject<USettings>(this, USettings::StaticClass());

  mouseSensitivity = settings->mouseSensitivity;
}
