// (c) 2023 Mika Pi

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
  : Mesh1P(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"))),
    FirstPersonCameraComponent(CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"))),
    bHasRifle(false)
{
  GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

  FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
  FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
  FirstPersonCameraComponent->bUsePawnControlRotation = true;

  Mesh1P->SetOnlyOwnerSee(true);
  Mesh1P->SetupAttachment(FirstPersonCameraComponent);
  Mesh1P->bCastDynamicShadow = false;
  Mesh1P->CastShadow = false;
  Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

auto AKitchenSinkCharacter::BeginPlay() -> void
{
  Super::BeginPlay();

  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController);

  // Add Input Mapping Context
  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
          playerController->GetLocalPlayer()))
  {
    Subsystem->AddMappingContext(DefaultMappingContext, 0);
  }

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

auto AKitchenSinkCharacter::move(const FInputActionValue &v) -> void
{
  CHECK_RET(GetController());

  const auto v2d = v.Get<FVector2D>();
  AddMovementInput(GetActorForwardVector(), v2d.Y);
  AddMovementInput(GetActorRightVector(), v2d.X);
}

auto AKitchenSinkCharacter::look(const FInputActionValue &v) -> void
{
  CHECK_RET(GetController());

  const auto v2d = v.Get<FVector2D>();
  AddControllerYawInput(v2d.X * mouseSensitivity);
  AddControllerPitchInput(v2d.Y * mouseSensitivity);
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
  auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
  if (!settings)
    settings = NewObject<USettings>(this, USettings::StaticClass());

  mouseSensitivity = settings->mouseSensitivity;
}

auto AKitchenSinkCharacter::GetFirstPersonCameraComponent() const -> const UCameraComponent *
{
  return FirstPersonCameraComponent;
}
