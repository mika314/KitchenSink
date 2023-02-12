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
  MuzzleOffset = vec(100.0f, 0.0f, 10.0f);
}

void UShelterWeaponComponent::Fire()
{
  CHECK_RET(character && character->GetController());

  CHECK_RET(ProjectileClass)

  auto playerController = Cast<APlayerController>(character->GetController());
  const auto SpawnRotation = playerController->PlayerCameraManager->GetCameraRotation();
  const auto SpawnLocation = getLoc(GetOwner()) + SpawnRotation.RotateVector(MuzzleOffset);

  FActorSpawnParameters ActorSpawnParams;
  ActorSpawnParams.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

  auto World = GetWorld();
  CHECK_RET(World)
  World->SpawnActor<AShelterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

  CHECK_RET(FireSound);
  UGameplayStatics::PlaySoundAtLocation(this, FireSound, getLoc(character));

  CHECK_RET(FireAnimation);
  auto AnimInstance = character->getMesh1P()->GetAnimInstance();
  CHECK_RET(AnimInstance);
  AnimInstance->Montage_Play(FireAnimation, 1.f);
}

void UShelterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  CHECK_RET(character);
  if (auto playerController = Cast<APlayerController>(character->GetController()))
  {
    CHECK_RET(playerController);
    auto inputSubsys =
      ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
    inputSubsys->RemoveMappingContext(FireMappingContext);
  }
}
