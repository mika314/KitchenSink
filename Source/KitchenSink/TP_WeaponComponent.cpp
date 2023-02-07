// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_WeaponComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "KitchenSinkCharacter.h"
#include "KitchenSinkProjectile.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
  // Default offset from the character location for projectiles to spawn
  MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire()
{
  LOG("fire");
  CHECK_RET(Character != nullptr && Character->GetController() != nullptr);

  // Try and fire a projectile
  CHECK_RET(ProjectileClass)
  UWorld *const World = GetWorld();
  CHECK_RET(World)

  APlayerController *PlayerController = Cast<APlayerController>(Character->GetController());
  const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
  // MuzzleOffset is in camera space, so transform it to world space before offsetting from the
  // character location to find the final muzzle position
  const FVector SpawnLocation =
    GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

  // Set Spawn Collision Handling Override
  FActorSpawnParameters ActorSpawnParams;
  ActorSpawnParams.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

  // Spawn the projectile at the muzzle
  World->SpawnActor<AKitchenSinkProjectile>(
    ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

  // Try and play the sound if specified
  if (FireSound != nullptr)
  {
    UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
  }

  // Try and play a firing animation if specified
  if (FireAnimation != nullptr)
  {
    // // Get the animation object for the arms mesh
    // UAnimInstance *AnimInstance = Character->GetMesh1P()->GetAnimInstance();
    // if (AnimInstance != nullptr)
    // {
    //   AnimInstance->Montage_Play(FireAnimation, 1.f);
    // }
  }
}

void UTP_WeaponComponent::AttachWeapon(AKitchenSinkCharacter *TargetCharacter)
{
  Character = TargetCharacter;
  if (Character == nullptr)
  {
    return;
  }

  // Attach the weapon to the First Person Character
  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
  AttachToComponent(TargetCharacter->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

  // switch bHasRifle so the animation blueprint can switch to another animation set
  TargetCharacter->SetHasRifle(true);

  // Set up action bindings
  if (APlayerController *PlayerController = Cast<APlayerController>(Character->GetController()))
  {
    if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer()))
    {
      // Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action
      // when using touch input
      Subsystem->AddMappingContext(FireMappingContext, 1);
    }

    if (UEnhancedInputComponent *EnhancedInputComponent =
          Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
    {
      // Fire
      EnhancedInputComponent->BindAction(
        FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
    }
  }
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  if (Character == nullptr)
  {
    return;
  }

  if (APlayerController *PlayerController = Cast<APlayerController>(Character->GetController()))
  {
    if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer()))
    {
      Subsystem->RemoveMappingContext(FireMappingContext);
    }
  }
}
