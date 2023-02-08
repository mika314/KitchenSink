// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShelterWeaponComponent.h"
#include "ShelterCharacter.h"
#include "ShelterProjectile.h"
#include <Camera/PlayerCameraManager.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>

UShelterWeaponComponent::UShelterWeaponComponent()
{
  MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UShelterWeaponComponent::Fire()
{
  LOG("fire");
  CHECK_RET(Character != nullptr && Character->GetController() != nullptr);

  CHECK_RET(ProjectileClass)

  auto PlayerController = Cast<APlayerController>(Character->GetController());
  const auto SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
  const auto SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

  FActorSpawnParameters ActorSpawnParams;
  ActorSpawnParams.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

  auto World = GetWorld();
  CHECK_RET(World)
  World->SpawnActor<AShelterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

  CHECK_RET(FireSound);
  UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());

  CHECK_RET(FireAnimation);
  auto AnimInstance = Character->GetMesh1P()->GetAnimInstance();
  CHECK_RET(AnimInstance);
  AnimInstance->Montage_Play(FireAnimation, 1.f);
}

void UShelterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  CHECK_RET(Character);

  auto PlayerController = Cast<APlayerController>(Character->GetController());
  CHECK_RET(PlayerController);
  auto Subsystem =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
  CHECK_RET(PlayerController);
  Subsystem->RemoveMappingContext(FireMappingContext);
}
