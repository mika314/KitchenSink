// (c) 2013 Mika Pi

#include "ShelterCharacter.h"
#include "ShelterHud.h"
#include "ShelterHudUi.h"
#include "ShelterWeaponComponent.h"
#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

AShelterCharacter::AShelterCharacter()
  : Mesh1P(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"))),
    FirstPersonCameraComponent(CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"))),
    shelterWeapon(CreateDefaultSubobject<UShelterWeaponComponent>(TEXT("shelterWeapon")))

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

  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
  shelterWeapon->AttachToComponent(Mesh1P, AttachmentRules, FName(TEXT("GripPoint")));
  shelterWeapon->Character = this;
}

void AShelterCharacter::BeginPlay()
{
  Super::BeginPlay();

  auto PlayerController = Cast<APlayerController>(GetController());
  CHECK_RET(PlayerController);

  auto Subsystem =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
  CHECK_RET(Subsystem);

  Subsystem->AddMappingContext(DefaultMappingContext, 0);

  CHECK_RET(shelterWeapon);
  Subsystem->AddMappingContext(shelterWeapon->FireMappingContext, 1);

  auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
  CHECK_RET(EnhancedInputComponent);

  EnhancedInputComponent->BindAction(
    shelterWeapon->FireAction, ETriggerEvent::Triggered, shelterWeapon, &UShelterWeaponComponent::Fire);

  hp = 1.f;
  shelterHp = 1.f;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setHp(getHp());
  hudUi->setShelterHp(shelterHp);
}

void AShelterCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
  auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
  CHECK_RET(EnhancedInputComponent);

  EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
  EnhancedInputComponent->BindAction(
    JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

  EnhancedInputComponent->BindAction(
    MoveAction, ETriggerEvent::Triggered, this, &AShelterCharacter::Move);

  EnhancedInputComponent->BindAction(
    LookAction, ETriggerEvent::Triggered, this, &AShelterCharacter::Look);
}

void AShelterCharacter::Move(const FInputActionValue &Value)
{
  const auto MovementVector = Value.Get<FVector2D>();
  CHECK_RET(Controller)

  AddMovementInput(GetActorForwardVector(), MovementVector.Y);
  AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AShelterCharacter::Look(const FInputActionValue &Value)
{
  const auto LookAxisVector = Value.Get<FVector2D>();
  CHECK_RET(Controller);

  AddControllerYawInput(LookAxisVector.X);
  AddControllerPitchInput(LookAxisVector.Y);
}

auto AShelterCharacter::getHp() const -> float
{
  return hp;
}

auto AShelterCharacter::applyDamage(float v) -> void
{
  hp -= v;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setHp(hp);
}

auto AShelterCharacter::getHudUi() -> UShelterHudUi *
{
  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController, nullptr);
  auto hud = Cast<AShelterHud>(playerController->GetHUD());
  CHECK_RET(hud, nullptr);
  return hud->getHudUi();
}

auto AShelterCharacter::addScrap() -> void
{
  scrap += rand() % 10 + 1;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setScrap(scrap);
}

auto AShelterCharacter::applyShelterDamage(float v) -> void
{
  shelterHp -= v * 0.01f;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setShelterHp(shelterHp);
}

auto AShelterCharacter::addMedkit() -> void
{
  ++medkits;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setMedkits(medkits);
}
